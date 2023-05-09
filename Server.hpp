/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:54:02 by nali              #+#    #+#             */
/*   Updated: 2023/05/09 15:05:22 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h> //addrinfo
#include <exception>
#include <cstring> //c_str()
#include <iostream>

class Server
{
    private:
        int port;
        int sockfd;
        std::string password;
        std::string server_ip;
        struct addrinfo *servinfo;
        
    public:
        Server();
        Server(int port, std::string pwd);
        Server(Server const &other);
        Server &operator= (Server const &other);
        ~Server();

    private:
        void CreateSocket(void);
        void LoadAddrinfo(void);
    
    public:
        class AddrInfoError: public std::exception //custom exception
        {
            private:
                int status;
            public :
                const char *what() const throw();
                AddrInfoError(int status); //constructor
        };
        class SocketError: public std::exception //custom exception
        {
            public :
                const char *what() const throw();
        };
      
    

        
    
};

#endif