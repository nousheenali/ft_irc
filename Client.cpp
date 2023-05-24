/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:46:25 by nali              #+#    #+#             */
/*   Updated: 2023/05/24 11:30:09 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{}

Client::Client(int fd)
{
    this->fd = fd;
    this->auth = false;
    
    struct sockaddr_in my_addr;
    socklen_t addr_len = sizeof(my_addr);
    getsockname(this->fd,(sockaddr *)&my_addr, &addr_len);
    inet_ntop(AF_INET, &my_addr.sin_addr, this->ip_addr, sizeof(ip_addr));
    this->port = ntohs(my_addr.sin_port); 
}

Client::~Client()
{}

Client Client::operator=(Client other)
{
    this->fd = other.fd;
    return (*this);
}

char *Client::get_ip_addr()
{   return (this->ip_addr); }

int Client::get_port()
{   return (this->port); }

int Client::get_socket()
{   return (this->fd); }

int Client::get_auth()
{   return (this->auth); }

void Client::set_auth(int val)
{   this->auth = val;}