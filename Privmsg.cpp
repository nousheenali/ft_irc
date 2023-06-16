/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:27:18 by nali              #+#    #+#             */
/*   Updated: 2023/06/12 10:59:02 by sfathima         ###   ########.fr       */
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
    if (params[0][0] == '#' || params[0][0] == '&'|| params[0][0] == '!' || params[0][0] == '+') // if msg to channel
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
            {
                std::string reply = ParamsJoin(params);
                if (reply[0] == ':')
                    reply = reply.substr(1); //removing the colon added by limechat
                serv->SendReply(rcvr->get_socket(), RPL_PRIVMSG(sdr->get_msg_prefix(), ch->get_channel_name(), reply));
            }
        }
    }
}

void privmsg::SendToClient()
{
    Client *sdr,*rcvr = NULL;
    std::string nick, username, ipaddr, host;
    size_t pos = params[0].find('!');
    size_t pos2 = params[0].find('@');
    size_t pos3 = params[0].find('%');
    
    if (pos != std::string::npos) //condition #1
    {
        nick = params[0].substr(0, pos);
        if (pos2 != std::string::npos)
        {
            username = params[0].substr(pos + 1, pos2 - pos - 1);
            ipaddr = params[0].substr(pos2 + 1);
            rcvr = serv->GetClient(nick);
            if (rcvr !=NULL && (rcvr->get_ip_addr() != ipaddr || rcvr->get_username() != username))
                rcvr = NULL;
        }
    }
    else if (pos3 != std::string::npos) //condition #2
    {
        nick = params[0].substr(0, pos3);
        host = params[0].substr(pos3 + 1);
        rcvr = serv->GetClient(nick);
        if (rcvr != NULL && rcvr->get_username() != host)
            rcvr = NULL;
    }
    else
        rcvr = serv->GetClient(params[0]);
        
    if (rcvr == NULL) //no user by the nick
    {
        serv->SendReply(sender_fd, ERR_NOSUCHUSER(serv->GetClient(sender_fd)->get_nickname(),params[0]));
        return ;
    }
    // std::cout << "user found....\n";
    if ((sdr = serv->GetClient(sender_fd)) != NULL)
    {   
        std::string reply = ParamsJoin(params);
        if (reply[0] == ':')
            reply = reply.substr(1); //removing the colon added by limechat
        serv->SendReply(rcvr->get_socket(), RPL_PRIVMSG( sdr->get_msg_prefix(), rcvr->get_nickname(), reply));
    }
}


/*  reference for reply
http://chi.cs.uchicago.edu/chirc/irc_examples.html */

/* condition #1
   PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
                                   ; Message to the user with nickname
                                   Wiz who is connected from the host
                                   tolsun.oulu.fi and has the username
                                   "jto".*/

/* condition #2
   PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
                                   ; Message to a user on the local
                                   server with username of "kalt", and
                                   connected from the host
                                   millennium.stealth.net.*/
