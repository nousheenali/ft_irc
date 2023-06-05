/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:27:18 by nali              #+#    #+#             */
/*   Updated: 2023/06/04 11:10:09 by nali             ###   ########.fr       */
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
    // Channel *ch = serv->GetChannel(params[0]);
    // if (ch == NULL) //channel doesn't exist
    // {
    //     serv->SendReply(sender_fd, ERR_NOSUCHCHANNEL(params[0]));
    //     return ;
    // }
    //if channel exists 
    // check if sender does not have rights to the channel
    // {
    //     // serv->SendReply(sender_fd, ERR_CANNOTSENDTOCHAN(msg[1]));
    //     // return ;
    // }
    //     //-------sender send msg to channel---------

}

void privmsg::SendToClient()
{
    int recipient_fd = -1;
    std::map<int, Client *>	client_list	= serv->GetAllClients();
    std::map<int, Client *>::iterator it = client_list.begin();
    
    while (it != client_list.end()) //get fd of the recipient with matching nick
    {
        if (it->second->get_nickname() == params[0])
        {
            recipient_fd = it->first;
            break;
        }
        it++;
    }
    if (recipient_fd == -1) //no user by the nick
    {
        serv->SendReply(sender_fd, ERR_NOSUCHCHANNEL(params[0]));
        return ;
    }
    //if matching user found, send msg
    std::cout << "user found....\n";
    Client *sdr = serv->GetClient(sender_fd);
    Client *rcvr = serv->GetClient(recipient_fd);
                                    //    _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
    // std::string text = ":" + sdr->get_nickname() + "!" + sdr->get_username() + "@" + sdr->get_ip_addr() + " PRIVMSG " + rcvr->get_nickname() + " :" + MessageJoin() +"\r\n";
    std::string text = sdr->get_msg_prefix() + " PRIVMSG " + rcvr->get_nickname() + " :" + ParamsJoin() +"\r\n";
    
    serv->SendReply(rcvr->get_socket(), text);
}

std::string privmsg::ParamsJoin()
{
    std::vector<std::string>::iterator it;
    std::string text;
    
    for (it = params.begin() + 1; it != params.end(); it++)
           text += *it + " ";
    return text;
}


/*  reference for reply
http://chi.cs.uchicago.edu/chirc/irc_examples.html */