/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:54:02 by nali              #+#    #+#             */
/*   Updated: 2023/05/12 08:48:44 by nali             ###   ########.fr       */
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
#include <unistd.h> //close
#include <stdlib.h> //malloc
#include <poll.h> 
#include <fcntl.h>
#include <arpa/inet.h> //inet_ntop
// #include <cstdlib>

#define BACKLOG 10 //no.of connections in incoming queue that waits till accept()
#define CLIENTS 5

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
        void BindListen(void);
        void ConnectClients(void);
        void AcceptConnections(struct pollfd *pfds, int *fd_count, int *maxfds);
        void ThrowException(std::string err_msg);
        void print_ip_addr(struct sockaddr *sa, int clientfd);
    
        class AddrInfoError: public std::exception //custom exception
        {
            private:
                int status;
            public :
                const char *what() const throw();
                AddrInfoError(int status); //constructor
        };
        class CustomException: public std::exception //custom exception
        {
            public :
                const char *what() const throw();
        };      
    

        
    
};

#endif