/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 11:13:18 by nali              #+#    #+#             */
/*   Updated: 2023/06/04 14:02:35 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string>  convert_to_vector(std::string msg);

void pong(Server *serv, int client_fd, msg_struct msg_info)
{
    std::vector<std::string> params = convert_to_vector(msg_info.parameter);
    if (params.size() == 0)
    {
        serv->SendReply(client_fd, ERR_NOORIGIN(msg_info.cmd));
        return ;
    }
    //a pong reply from client simply means the client is active
    //this is applicable only if we implement ping command
    
}