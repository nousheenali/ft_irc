/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:54:02 by nali              #+#    #+#             */
/*   Updated: 2023/05/24 14:33:49 by sfathima         ###   ########.fr       */
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
#include <vector>
#include <map>
#include "Client.hpp"
#include "Replies.hpp"
#include "Command.hpp"
#include "Mode.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"    
#define GREEN   "\033[32m"     
#define YELLOW  "\033[33m"     
#define BLUE    "\033[34m"      
#define BACKLOG 10 //no.of connections in incoming queue that waits till accept()

class Server
{
    private:
        int port;
        int listener;
        std::string password;
        std::string server_ip;
        std::string server_name;
        struct addrinfo *servinfo;
        std::vector<pollfd> pfds; //vector to store poll fds
        int pfd_count;   //to store file descriptors to be polled
        std::map<int, Client *> client_array; 
        std::map<std::string, Channel *> channel_array; 
		Command *cmd_cls;
        
    public:
        Server();
        Server(int port, std::string pwd);
        Server(Server const &other);
        Server &operator= (Server const &other);
        ~Server();
        Client* GetClient(int client_fd);
        Channel* GetChannel(std::string name);

    private:
        void CreateSocket(void);
        void LoadAddrinfo(void);
        void Listen(void);
        void ConnectClients(void);
        void AcceptConnections();
        void ThrowException(std::string err_msg);
        void ReceiveMessage(int i);
        void MessageStoreExecute(char c, int client_fd);
        void print_messages(int fd);
		int	 check_auth(int fd);
        void close_fds();
        void SendReply(int client_fd, std::string msg);
    
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