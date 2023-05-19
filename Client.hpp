/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:34:33 by nali              #+#    #+#             */
/*   Updated: 2023/05/19 10:21:55 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <netdb.h> //addrinfo
#include <arpa/inet.h> //inet_ntop
#include <vector> //inet_ntop

class Client
{
    private:
        int fd; //socket
        bool auth; //set to true when authenticated by server
        char ip_addr[INET6_ADDRSTRLEN];
        int port;
        std::string nick;
        
    public:
        std::vector <std::string> message;
        
    public:
        Client();
        Client(int fd);
        ~Client();
        Client operator=(Client other);
        char *get_ip_addr();
        int get_port();
        int get_socket();
        
};

#endif