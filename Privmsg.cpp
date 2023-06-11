/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:27:18 by nali              #+#    #+#             */
/*   Updated: 2023/06/11 11:02:14 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Privmsg.hpp"

privmsg::privmsg(Server *serv, int sender_fd, msg_struct msg_info)
{   
    this->sender_fd = sender_fd;
    this->serv = serv;
    this->params = convert_to_vector(msg_info.parameter);
    this->msg_info = &msg_info;
    CheckConditions();
}

void privmsg::CheckConditions()
{
    if (params.size() == 0) //if no arguments
    {
        serv->SendReply(sender_fd, ERR_NORECIPIENT(msg_info->cmd));
        return ;
    }
    else if (params.size() == 1) //if only name given but no message
    {
        serv->SendReply(sender_fd, ERR_NOTEXTTOSEND());
        return ;
    }
    if (params[0][0] == '#' || params[0][0] == '&') // if msg to channel
        SendToChannel(); 
    else //if msg to user
        SendToClient();
}

void privmsg::SendToChannel()
{
    Client *sdr, *rcvr;
    Channel *ch = serv->GetChannel(params[0]);
    
    if (ch == NULL) //channel doesn't exist
    {
        serv->SendReply(sender_fd, ERR_NOSUCHCHANNEL(serv->GetClient(sender_fd)->get_nickname(), params[0]));
        return ;
    }
    // std::cout << "channel found....\n";
    if ((sdr = serv->GetClient(sender_fd)) != NULL)
    {
        if (ch->isMember(sdr->get_nickname()) == false) //sender not a member of channel
        {
            serv->SendReply(sender_fd, ERR_CANNOTSENDTOCHAN(params[0]));
            return ;
        }
        for(size_t i = 0; i < ch->members.size(); i++)
        {
            rcvr = ch->members[i].user;
            if (rcvr->get_nickname() != sdr->get_nickname())
                serv->SendReply(rcvr->get_socket(), RPL_PRIVMSG(sdr->get_msg_prefix(), ch->get_channel_name(), ParamsJoin(params)));
        }
    }
}

void privmsg::SendToClient()
{
    // int recipient_fd = -1;
    Client *sdr,*rcvr;

    rcvr = serv->GetClient(params[0]);
    if (rcvr == NULL) //no user by the nick
    {
        serv->SendReply(sender_fd, ERR_NOSUCHCHANNEL(serv->GetClient(sender_fd)->get_nickname(),params[0]));
        return ;
    }
    // std::cout << "user found....\n";
    if ((sdr = serv->GetClient(sender_fd)) != NULL)
        serv->SendReply(rcvr->get_socket(), RPL_PRIVMSG( sdr->get_msg_prefix(), rcvr->get_nickname(), ParamsJoin(params)));
}


/*  reference for reply
http://chi.cs.uchicago.edu/chirc/irc_examples.html */