/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:34:33 by nali              #+#    #+#             */
/*   Updated: 2023/05/25 22:03:10 by nali             ###   ########.fr       */
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
        
    public:
        std::string nick;
        std::vector <std::string> message;
        // std::vector <std::vector<std::string> > message;
        
    public:
        Client();
        Client(int fd);
        ~Client();
        Client operator=(Client other);
        char *get_ip_addr();
        int get_port();
        int get_socket();
        int get_auth();
        void set_auth(int val);
        
};

#endif