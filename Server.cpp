/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:53:47 by nali              #+#    #+#             */
/*   Updated: 2023/05/09 15:07:07 by nali             ###   ########.fr       */
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
    LoadAddrinfo();
    if ((this->sockfd = socket(this->servinfo->ai_family, this->servinfo->ai_socktype, this->servinfo->ai_protocol)) == -1)
    {            
        perror("Socket Error:");
        throw SocketError();
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
const char *Server::AddrInfoError::what() const throw()
{
    return (gai_strerror(this->status));
}

Server::AddrInfoError::AddrInfoError(int status) //exception constructor
{
    this->status = status;
}

const char *Server::SocketError::what() const throw()
{
    return ("Socket Error");
}

