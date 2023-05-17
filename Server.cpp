/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:53:47 by nali              #+#    #+#             */
/*   Updated: 2023/05/16 19:08:14 by nali             ###   ########.fr       */
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
    if (p  == NULL)
        ThrowException("Socket Bind Error: ");
}

void Server::Listen(void)
{
    if (listen(this->listener, BACKLOG) == -1)
    {
        close(this->listener);
        ThrowException("Socket Listen Error: ");
    }
}

// void Server::VerifyPwd(int clientfd)
// {
//     char input[256];
//     std::string str, substr;
//     size_t pos;
    
//     send(clientfd, "Connecting to ft_irc server......\n",34,0);
//     do
//     {
//         memset(&input, 0, sizeof(input));
//         send(clientfd, "Please enter password : ",24,0);
//         recv(clientfd, &input, 10, 0);
//         // if (recv(clientfd, &input, 10, 0) == -1)
//         //     std::cout <<"error 0000000000000000\n";
//         pos = strcspn(input, "\n"); //find position of newline
//         str = std::string(input);
    
//         substr = str.substr(0, pos);
//         if (substr == this->password)
//             break;
//         send(clientfd, "Incorrect Password. Try Again!\n",31,0);
//     }while( substr != this->password);
      
//     send(clientfd, "Password verfication Successful.\n",33,0);
//     send(clientfd, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++",70,0);
//     send(clientfd,"\n",1,0);
//     send(clientfd, "                       Welcome To ft_irc server",48,0);
//     send(clientfd,"\n",1,0);
//     send(clientfd, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++",70,0);
//     send(clientfd,"\n",1,0);
// }

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
        send(clientfd, "Connection established...\n",26,0);
        pfdStruct.fd = clientfd;
        pfdStruct.events = POLLIN;
        c = new Client(clientfd);
        client_array.push_back(c);
        this->pfds.push_back(pfdStruct);
        this->pfd_count += 1;
        std::cout << "Server got connection from " << c->get_ip_addr() << " on socket " << c->get_socket() <<std::endl;
    }
}

std::string Server::ReceiveMessage(int i, std::string str)
{
    char buf[1];
    int nbytes, sender_fd;
    
    memset(&buf, 0, sizeof(buf));
    nbytes = recv(this->pfds[i].fd, buf, sizeof(buf), 0);
    sender_fd = pfds[i].fd;
    if (nbytes == 0)
    {
        std::cout << " *** Connection Closed by Client *** \n";
        if (close(sender_fd) == -1)
            ThrowException("Close Error: ");
        pfds[i].fd = -1; //make the socket fd negative so it is ignored in future
    }
    
    else if (nbytes < 0)
        ThrowException("recv Error: ");
        
    else
    {
        // std::cout << "buf is " << buf <<"\n";
        if(*buf != '\n')
        str.push_back(*buf);
        // std::cout << "str is " << str <<"\n";
        // for (int j = 0; j < this->pfd_count; j++)
        // {
        //     int dest_id = pfds[j].fd;
        //     if (dest_id != this->listener && dest_id != sender_fd)
        //     {
        //         if (send(dest_id, buf, nbytes, 0)  == -1)
        //             ThrowException("send Error: ");
        //     }
        // }
        return (str);
    }
}

void Server::ConnectClients(void)
{
    pollfd pfdStruct; // to store socket information
    this->pfd_count = 0;
    std::string str;

    //adding listener socket to list of poll fds
    pfdStruct.fd = this->listener; 
    pfdStruct.events = POLLIN; //for input operations are possible on this fd
    this->pfd_count += 1;
    this->pfds.push_back(pfdStruct);
    while(1)
    {
        // std::cout << "looping\n";
        int val = poll(&this->pfds[0], this->pfd_count, 5000); //returns no.of elements in pollfds whose revents has been set to a nonzero value 
        if (val < 0)
            ThrowException("Poll Error: ");
        for (int i = 0; i < this->pfd_count; i++) //going through each socket to check for incoming requests
        {
            // std::cout << "res ->" << pfds[i].revents << ", i = " << i << "fd = " << pfds[i].fd <<"\n";
            if (pfds[i].revents & POLLIN) //checking revents if it is set to POLLIN
            {
                if (pfds[i].fd == this->listener)
                    AcceptConnections();
                else
                    str = ReceiveMessage(i, str);
            }
        }
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
    if ((status = getaddrinfo("127.0.0.1", port_str.c_str(), &hints, &this->servinfo)) != 0) 
    { 
       throw AddrInfoError(status);
    }
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