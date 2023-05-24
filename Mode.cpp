/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:56 by nali              #+#    #+#             */
/*   Updated: 2023/05/24 13:49:33 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

Mode::Mode()
{}

Mode::Mode(int client_fd, Server *serv)
{
    Client *c;
    Channel *chl;
    std::vector<std::string> msg;
    
    c = serv->GetClient(client_fd);
    msg = c->message;
    chl = serv->GetChannel(msg[1]);
    // if (chl == NULL) //channel doesn'


    // it = serv->channel_array.find();
    // if (it == serv->channels.end())
    // {
    //     serv->SendReply(client_fd,ERR_NOSUCHCHANNEL(msg[1]));
    //     return ;
    // }
    

    
    
    //
}