/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:19:36 by nali              #+#    #+#             */
/*   Updated: 2023/06/14 10:25:45 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICE_HPP
#define NOTICE_HPP

#include "Server.hpp"

std::vector<std::string>  convert_to_vector(std::string msg);
std::string ParamsJoin(std::vector<std::string> vec);

class notice
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
        notice(Server *serv, int sender_fd, msg_struct msg_info);
};

#endif