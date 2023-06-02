/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:19:36 by nali              #+#    #+#             */
/*   Updated: 2023/06/02 12:05:40 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "Server.hpp"

class Privmsg
{
     private:
        int sender_fd;
        Client *sender_client;
        Server *serv;
        Channel *chl;
        std::vector<std::string> msg;
    
    public:
        Privmsg(int client_fd, Server *serv);
        void CheckConditions(void);
        void SendToChannel();
        void SendToClient();
        std::string MessageJoin();
};

#endif