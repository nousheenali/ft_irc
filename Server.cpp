/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:53:47 by nali              #+#    #+#             */
/*   Updated: 2023/05/25 14:10:13 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{}

Server::~Server()
{}

Server::Server(int port, std::string pwd)
{
    this->port = port;
    this->password = pwd;
    this->server_ip = "127.0.0.1";
    this->server_name = "ft_irc";
    this->pfd_count = 0;
    
    this->CreateSocket();
    this->Listen();
    this->ConnectClients();
}

Server &Server::operator=(Server const &other)
{
    this->port = other.port;
    this->password = other.password;
    this->password = other.pfd_count;
    return (*this);
}

Server::Server(Server const &other)
{
    *this = other;
}

void Server::CreateSocket(void)
{
    struct addrinfo *p;
    int optval = 1;
    LoadAddrinfo();
    for (p = this->servinfo; p != NULL; p = p->ai_next) 
    {
        if ((this->listener = socket(this->servinfo->ai_family, this->servinfo->ai_socktype, this->servinfo->ai_protocol)) == -1)
        {            
            perror("Socket Error: ");
            continue; //skip to next entry on addrinfo struct
        } 
        
        // to make the port available for reuse ref:comment #1
        if (setsockopt(this->listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
        {
            freeaddrinfo(this->servinfo);
            close(this->listener);
            ThrowException("SetSockOpt Error: ");
        }
        
        fcntl(this->listener, F_SETFL, O_NONBLOCK);
        if (bind(this->listener, this->servinfo->ai_addr, this->servinfo->ai_addrlen) == -1)
        {
            close(this->listener);
            perror("Socket Bind Error: ");
            continue; //skip to next entry on addrinfo struct
        }
        
        break; //break out of the loop if valid socket found
    }
    
    freeaddrinfo(this->servinfo);
    if (p  == NULL) // in case of any other error
    {
        close(this->listener);
        ThrowException("Socket Error: ");
    }
}

void Server::LoadAddrinfo(void)
{
    int status = -1;
    struct addrinfo hints;
    
    memset(&hints, 0 , sizeof(hints));
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // type is stream socket
    // hints.ai_flags = AI_PASSIVE; // fills my ip address
        
    std::string port_str = std::to_string(this->port); // convert port from int and then to char * with c_str()
    if ((status = getaddrinfo(server_ip.c_str(), port_str.c_str(), &hints, &this->servinfo)) != 0) 
    { 
       throw AddrInfoError(status);
    }
}

void Server::Listen(void)
{
    if (listen(this->listener, BACKLOG) == -1)
    {
        if (close(this->listener) == -1)
            ThrowException("FD Close Error: ");
        ThrowException("Socket Listen Error: ");
    }
}

void Server::ConnectClients(void)
{
    pollfd pfdStruct; // to store socket information

    //adding listener socket to list of poll fds
    pfdStruct.fd = this->listener; 
    pfdStruct.events = POLLIN; //for input operations are possible on this fd
    this->pfd_count += 1;
    this->pfds.push_back(pfdStruct);
    while(1)
    {
        int val = poll(&this->pfds[0], this->pfd_count, 5000); //returns no.of elements in pollfds whose revents has been set to a nonzero value 
        if (val < 0)
        {
            //close all fds
            close_fds();
            ThrowException("Poll Error: ");
        }
        for (int i = 0; i < this->pfd_count; i++) //going through each socket to check for incoming requests
        {
            // std::cout << "res ->" << pfds[i].revents << ", i = " << i << "fd = " << pfds[i].fd <<"\n";
            if (pfds[i].revents & POLLIN) //checking revents if it is set to POLLIN
            {
                if (pfds[i].fd == this->listener)
                    AcceptConnections();
                else
                    ReceiveMessage(i);
            }
        }
    }
}

void Server::AcceptConnections()
{
    pollfd pfdStruct;
    std::string str;
    int clientfd;
    socklen_t addr_len;
    struct sockaddr_storage client_addr; // to store clients details
    Client *c;

    
    addr_len = sizeof(struct sockaddr_storage);
    clientfd = accept(this->listener, (struct sockaddr *)&client_addr, &addr_len);
    if (clientfd == -1)
        ThrowException("Accept Error: ");
    else
    {
        pfdStruct.fd = clientfd;
        pfdStruct.events = POLLIN;
        c = new Client(clientfd);
        client_array.insert(std::make_pair<int, Client *>(clientfd, c));
        this->pfds.push_back(pfdStruct);
        this->pfd_count += 1;
        std::cout << YELLOW <<" *** Server got connection from " << c->get_ip_addr() << " on socket " << c->get_socket() << " *** "<< RESET <<std::endl;
    }
}

void Server::ReceiveMessage(int i)
{
    char buf[1];
    int nbytes, sender_fd;
    
    memset(&buf, 0, sizeof(buf));
    nbytes = recv(this->pfds[i].fd, buf, sizeof(buf), 0);
    sender_fd = pfds[i].fd;
    if (nbytes <= 0)
    {
        std::cout << RED <<" *** Connection Closed by Client *** \n" << RESET ;
        if (close(sender_fd) == -1)
            ThrowException("FD Close Error: ");
        client_array.erase(pfds[i].fd); // remove the client from the array
        pfds[i].fd = -1; //make the socket fd negative so it is ignored in future
        return;
    }        
    else
        MessageStoreExecute(buf[0], pfds[i].fd);
}

void Server::MessageStoreExecute(char ch, int client_fd)
{
    std::map<int, Client *>::iterator it;
    static std::string tmp;
    // static std::vector<std::string> vec;
    Channel *chl;
    
    if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '/' )
        tmp += ch;
    else if (ch == ' ' || ch == '\n')
    {
        it = client_array.find(client_fd);
        if (it != client_array.end())
        {
            it->second->message.push_back(tmp);
            // std::cout << "tmp is " << tmp <<"\n";
            tmp.clear();
            if (ch == '\n')
            {
                //parse and process request here
                print_messages(client_fd);
                it->second->nick = "nali";
                if (it->second->get_auth() == 0)
                {
					int ret = check_auth(client_fd);
					if (ret == 0)
					{
                    	SendReply(client_fd, RPL_WELCOME(it->second->nick));
                    	it->second->set_auth(1);
					}
                    else if (ret == -1)
                    {
                        SendReply(client_fd, ERR_UNKNOWNCOMMAND(it->second->message[0]));
                        it->second->message.clear();
                        return;
                    }
					else
					{
						// execute_cmds
					}
                }
                if (it->second->get_auth() == 1)
                {
                    if (it->second->message[0] == "MODE")
                    {
                        chl = new Channel("chl1");
                        this->channel_array.insert(std::make_pair("chl1", chl));
                        chl = new Channel("chl2");
                        this->channel_array.insert(std::make_pair("chl2", chl));
                        Mode(client_fd, this);
                    }
                }
                it->second->message.clear();
            }
        }
    }
}

void Server::print_messages(int fd)
{
    Client *c;
    int size;
    
    c = GetClient(fd);
    if (c != NULL)
    {
        size = c->message.size();
        for (int j= 0; j < size; j++)
            std::cout << c->message[j] << " ";
        std::cout << "\n---------------------\n";
    }
}

int Server::check_auth(int fd)
{
	Client *c;
	std::map<int, Client *>::iterator it;

	// it = client_array.find(fd);
	c = GetClient(fd);
	if (c != NULL)
    {
        if (c->message.size() < 1)
            SendReply(fd, ERR_NEEDMOREPARAMS(c->message[0]));
		if (c->message[0] == "PASS" || c->message[0] == "NICK" || c->message[0] == "USER")
        {
            if (c->message[0] == "PASS" && c->message[1] == this->password)
			    return (0);
            else if ((c->message[0] == "PASS" && c->message[1] != this->password))
                return (-1);
			else
				return (1);
        }
        // std::cout << "cmd is PASS but password incorrect" << this->password << it->second->message[1] << "\n";
	}
	return (1);
}

void Server::SendReply(int client_fd, std::string msg)
{
    send(client_fd, msg.c_str(), msg.length(), 0);
}

// void Server::print_messages(int fd)
// {
//     std::map<int, Client *>::iterator it;
//     int size;

//     it = this->client_array.find(fd);
//     if (it != client_array.end())
//     {
//         size = it->second->message.size();
//         for (int j = 0; j < size; j++)
//             std::cout << it->second->message[j] << "\n";
//         std::cout << "-------------------\n";
//     }
    
// }

void Server::close_fds()
{
    std::cout << "close fd called \n";
    for (int i = 0; i < pfds.size(); i++)
    {
        if (pfds[i].fd > 0)  // closed client connections are set to -1 so this check
            if (close(pfds[i].fd) == -1)
                ThrowException("FD Close Error: ");
    }
}


Client* Server::GetClient(int client_fd)
{
    std::map<int, Client*>::iterator it;
    it = this->client_array.find(client_fd);
    if (it != client_array.end())
        return (it->second);
    return (NULL);
}

Channel* Server::GetChannel(std::string name)
{
    std::map<std::string, Channel*>::iterator it;
    it = this->channel_array.find(name);
    if (it != channel_array.end())
        return (it->second);
    return (NULL);
}

//exceptions
void Server::ThrowException(std::string err_msg)
{
    std::cout << err_msg;
    throw CustomException();
}

const char *Server::AddrInfoError::what() const throw()
{
    return (gai_strerror(this->status));
}

Server::AddrInfoError::AddrInfoError(int status) //exception constructor
{
    this->status = status;
}

const char *Server::CustomException::what() const throw()
{
    return (strerror(errno));
}


/* Comment #1

SO_REUSEADDR -  Allows other sockets to bind() to this port, unless there is an active 
                listening socket bound to the port already. This enables you to get 
                around those “Address already in use” error messages when you try to 
                restart your server after a crash.

SOL_SOCKET   -  To manipulate options at the sockets API level. To manipulate options 
                at any other level the protocol number of the appropriate protocol 
                controlling the option is supplied. For example, to indicate that an 
                option is to be interpreted by the TCP protocol, level should be set 
                to the protocol number of TCP
                
optval       -  To access option values for setsockopt. The parameter should be non-zero 
                to enable a boolean option,or zero if the option is to be disabled.

*/

/*  Why there are multiple entires in addrinfo struct?
    the network host is multihomed, accessible over multiple protocols 
    (e.g., both AF_INET and AF_INET6); or the same service is available 
    from multiple socket types (eg: SOCK_STREAM address or SOCK_DGRAM address).
    Therefore, the host could have multiple ways to contact it, and getaddrinfo 
    is listing all known ways.
*/

/* The fcntl() file control function provides for control over open file descriptor.*/


/* Select() Poll() and Epoll()
    https://www.youtube.com/watch?v=dEHZb9JsmOU 
    
    struct pollfd
    {
        int fd;         //file desciptor
        short events;   //requested events
        short revents;  //returned events
    };
*/

/* Message format to client
    
    :server 001 <nick> :Welcome to the <network> Network, <nick>[!<user>@<host>]
    <nick>: Nickname of the connecting user.
    <network>: Network name.
    <user>@<host>: Username and hostname of the connecting user.
    
    https://dd.ircdocs.horse/refs/numerics/001.html
    https://www.rfc-editor.org/rfc/rfc2812
    
*/
