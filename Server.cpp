/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:53:47 by nali              #+#    #+#             */
/*   Updated: 2023/05/17 12:31:19 by nali             ###   ########.fr       */
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

void Server::ReceiveMessage(int i)
{
    char buf[1];
    int nbytes, sender_fd;
    static std::string tmp;
    static std::vector<std::string> vec_tmp;
    
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
        if (buf[0] != ' ' && buf[0] != '\n')
            tmp += buf[0];
        // std::cout << "tmp is " << tmp <<"\n";
        else if (buf[0] == ' ' || buf[0] == '\n')
        {
            vec_tmp.push_back(tmp);
            tmp.clear();
            if (buf[0] == '\n')
            {
                cmds.push_back(vec_tmp);
                vec_tmp.clear();
                print_messages();
            }
        }
    }
    // return buf[0];
}

void Server::print_messages()
{
    //printing the input messages recieved
    for (int j = 0; j < cmds.size(); j++)
        for (int k = 0; k < cmds[j].size(); k++)
            std::cout << cmds[j][k] <<"\n";
    std::cout << "-------------------\n";
    
}

void Server::ConnectClients(void)
{
    pollfd pfdStruct; // to store socket information
    this->pfd_count = 0;
    std::string str;
    // std::string tmp;
    // std::vector<std::string> vec_tmp;
    // char c;

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
                    ReceiveMessage(i);
            }
        }
    }
}
   
// void Server::ConnectClients(void)
// {
//     pollfd pfdStruct; // to store socket information
//     this->pfd_count = 0;
//     std::string str;
//     std::string tmp;
//     std::vector<std::string> vec_tmp;
//     char c;

//     //adding listener socket to list of poll fds
//     pfdStruct.fd = this->listener; 
//     pfdStruct.events = POLLIN; //for input operations are possible on this fd
//     this->pfd_count += 1;
//     this->pfds.push_back(pfdStruct);
//     while(1)
//     {
//         // std::cout << "looping\n";
//         int val = poll(&this->pfds[0], this->pfd_count, 5000); //returns no.of elements in pollfds whose revents has been set to a nonzero value 
//         if (val < 0)
//             ThrowException("Poll Error: ");
//         for (int i = 0; i < this->pfd_count; i++) //going through each socket to check for incoming requests
//         {
//             // std::cout << "res ->" << pfds[i].revents << ", i = " << i << "fd = " << pfds[i].fd <<"\n";
//             if (pfds[i].revents & POLLIN) //checking revents if it is set to POLLIN
//             {
//                 if (pfds[i].fd == this->listener)
//                     AcceptConnections();
//                 else
//                 {
//                     c = ReceiveMessage(i);
//                     if (c != ' ' && c != '\n')
//                         tmp += c;
//                     else if (c == ' ' || c == '\n')
//                     {
//                         vec_tmp.push_back(tmp);
//                         tmp.clear();
//                         if (c == '\n')
//                         {
//                             cmds.push_back(vec_tmp);
//                             vec_tmp.clear();
//                             //printing the input messages recieved
//                             for (int j = 0; j < cmds.size(); j++)
//                                 for (int k = 0; k < cmds[j].size(); k++)
//                                     std::cout << cmds[j][k] <<"\n";
//                             std::cout << "-------------------\n";
//                         }
//                     }
//                 }
                    
//             }
//         }
//     }
// }
    

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