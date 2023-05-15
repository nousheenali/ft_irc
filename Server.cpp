/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:53:47 by nali              #+#    #+#             */
/*   Updated: 2023/05/12 09:19:13 by nali             ###   ########.fr       */
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
    this->CreateSocket();
    this->BindListen();
    this->ConnectClients();
}

Server &Server::operator=(Server const &other)
{
    *this = other;
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
    if ((this->sockfd = socket(this->servinfo->ai_family, this->servinfo->ai_socktype, this->servinfo->ai_protocol)) == -1)
    {            
        freeaddrinfo(this->servinfo);
        ThrowException("Socket Error: ");
    } 
    fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
    // to make the port available for reuse ref:comment #1
    if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        freeaddrinfo(this->servinfo);
        ThrowException("SetSockOpt Error: ");
    }
}

void Server::BindListen(void)
{
    if (bind(this->sockfd, this->servinfo->ai_addr, this->servinfo->ai_addrlen) == -1)
    {
        close(this->sockfd);
        freeaddrinfo(this->servinfo);
        ThrowException("Socket Bind Error: ");
    }
    if (listen(sockfd, BACKLOG) == -1)
    {
        // close(this->sockfd);
        freeaddrinfo(this->servinfo);
        ThrowException("Socket Listen Error: ");
    }
    freeaddrinfo(this->servinfo);
}

void Server::print_ip_addr(struct sockaddr *sa, int clientfd)
{
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_in *ptr;
    struct sockaddr_in6 *ptr1;
    
    if (sa->sa_family == AF_INET)
    {
        ptr = (struct sockaddr_in *)&sa;
        inet_ntop(AF_INET, (struct sockaddr*)&ptr->sin_addr, s, sizeof s);
        std::cout << "Server got connection from " << s << " on socket " << clientfd <<std::endl;
    }
    else if (sa->sa_family == AF_INET6)
    {
        ptr1 = (struct sockaddr_in6 *)&sa;
        inet_ntop(AF_INET, (struct sockaddr*)&ptr1->sin6_addr, s, sizeof s);
        std::cout << "Server got connection from " << s << " on socket " << clientfd <<std::endl;
    }
    else
        std::cout << "Address family is neither AF_INET or AF_INET6";
}

void Server::AcceptConnections(struct pollfd *pfds, int *fd_count, int *maxfds)
{
    int clientfd;
    socklen_t addr_len;
    struct sockaddr_storage client_addr; // to storage clients details
    
    addr_len = sizeof(struct sockaddr_storage);
    clientfd = accept(this->sockfd, (struct sockaddr *)&client_addr, &addr_len);
    if (clientfd == -1)
        ThrowException("Accept Error: ");
    else
    {
        if (fd_count == maxfds) //if number of clients exceeds max value
        {
            *maxfds *= 2; //doubling the capacity
            if ((pfds = (struct pollfd *)realloc(pfds, *maxfds * sizeof(struct pollfd))) == NULL)
                ThrowException("Malloc Error: ");
        }
        pfds[*fd_count].fd = clientfd;
        pfds[*fd_count].events = POLLIN;
        *fd_count += 1;
        print_ip_addr((struct sockaddr *)&client_addr, clientfd);
    }
}

void Server::ConnectClients(void)
{
    struct pollfd *pfds; // to store socket information
    int maxfds = CLIENTS;
    int fd_count = 0;
    char buf[256];

    if ((pfds  = (struct pollfd *)malloc(CLIENTS * sizeof(struct pollfd))) == NULL)
        ThrowException("Malloc Error: ");
    //adding listener socket
    if (CLIENTS > 0)
    {
        pfds[0].fd = this->sockfd; 
        pfds[0].events = POLLIN; //for input operations are possible on this fd
        fd_count++;
    }
    while(1)
    {
        std::cout << "looping\n";
        int val = poll(&pfds[0], fd_count, 5000);
        if (val == -1)
        {
            std::cout << "Poll Error: ";
            throw CustomException();
        }
        std::cout << "fd_count is" << fd_count <<std::endl;
        for (int i = 0; i < fd_count; i++) //going through each socket to check for incoming requests
        {
            if (pfds[i].revents & POLLIN) //checking revents if it is set to POLLIN
            {
                if (pfds[i].fd == this->sockfd)
                    AcceptConnections(pfds, &fd_count, &maxfds);
                else
                {
                    int nbytes = recv(pfds[i].fd, buf, sizeof(buf), 0);
                    int sender_fd = pfds[i].fd;
                    if (nbytes == 0)
                    {
                        std::cout << "Connection Closed by client\n";
                        if (close(sender_fd) == -1)
                            ThrowException("Close Error: ");
                        pfds[i].fd = -1; //make the socket fd negative so it is ignored in future
                    }
                    else if (nbytes < 0)
                        ThrowException("recv Error: ");
                    else
                    {
                        buf[nbytes] = '\0';
                        std::cout << "-----" << buf << "----" <<std::endl;;
                        // for (int j = 0; j < fd_count; j++)
                        // {
                            // int dest_id = pfds[j].fd;
                            // if (dest_id != this->sockfd && dest_id != sender_fd)
                            // {
                            //     if (send(dest_id, buf, nbytes, 0)  == -1)
                            //         ThrowException("send Error: ");
                            // }
                        // }
                        std::cout << "testing\n";
                    }   
                }
                
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
    hints.ai_flags = AI_PASSIVE; // fills my ip address
        
    std::string port_str = std::to_string(this->port); // convert port from int and then to char * with c_str()
    if ((status = getaddrinfo(NULL, port_str.c_str(), &hints, &this->servinfo)) != 0) 
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