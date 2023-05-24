/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:56 by nali              #+#    #+#             */
/*   Updated: 2023/05/24 12:47:13 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

Mode::Mode()
{}

Mode::Mode(int client_fd, Server *serv)
{
    // if channel does not exist
    // std::cout <<"here...." <<msg[1] << std::endl;
    std::vector<std::string> msg;
    std::map<std::string, Channel *>::iterator it;

    // it = serv->channel_array.find();
    // if (it == serv->channels.end())
    // {
    //     serv->SendReply(client_fd,ERR_NOSUCHCHANNEL(msg[1]));
    //     return ;
    // }
    

    
    
    //
}