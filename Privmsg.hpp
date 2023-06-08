/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:19:36 by nali              #+#    #+#             */
/*   Updated: 2023/06/08 12:06:05 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "Server.hpp"

std::vector<std::string>  convert_to_vector(std::string msg);
std::string ParamsJoin(std::vector<std::string> vec);

class privmsg
{
     private:
        int sender_fd;
        Server *serv;
        std::vector<std::string> params;
        msg_struct *msg_info;
        
    private:
        void CheckConditions();
        void SendToChannel();
        void SendToClient();
    public:
        privmsg(Server *serv, int sender_fd, msg_struct msg_info);
};

#endif