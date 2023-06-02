/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:27:18 by nali              #+#    #+#             */
/*   Updated: 2023/06/02 12:09:58 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Privmsg.hpp"

Privmsg::Privmsg(int sender_fd, Server *serv)
{   
    this->sender_fd = sender_fd;
    this->serv = serv;
    this->sender_client = serv->GetClient(sender_fd);
    this->msg = sender_client->message;
    CheckConditions();
}

void Privmsg::CheckConditions(void)
{
    if (msg.size() == 1) //if no arguments
    {
        serv->SendReply(sender_fd, ERR_NORECIPIENT(msg[0]));
        return ;
    }
    else if (msg.size() == 2) //if only name given but no message
    {
        serv->SendReply(sender_fd, ERR_NOTEXTTOSEND());
        return ;
    }
    if (msg[1][0] == '#' || msg[1][0] == '&') // if msg to channel
        SendToChannel(); 
    else //if msg to user
        SendToClient();
}

void Privmsg::SendToChannel()
{
    Channel *ch = serv->GetChannel(msg[1]);
    if (ch == NULL) //channel doesn't exist
    {
        serv->SendReply(sender_fd, ERR_NOSUCHCHANNEL(msg[1]));
        return ;
    }
    //if channel exists 
    // check if sender does not have rights to the channel
    // {
    //     // serv->SendReply(sender_fd, ERR_CANNOTSENDTOCHAN(msg[1]));
    //     // return ;
    // }
    //     //-------sender send msg to channel---------

}

void Privmsg::SendToClient()
{
    int recipient_fd = -1;
    std::map<int, Client *>	client_list	= serv->GetAllClients();
    std::map<int, Client *>::iterator it = client_list.begin();
    while (it != client_list.end()) //get fd of the recipient with matching nick
    {
        if (it->second->get_nickname() == msg[1])
        {
            recipient_fd = it->first;
            break;
        }
    }
    if (recipient_fd == -1) //no user by the nick
    {
        serv->SendReply(sender_fd, ERR_NOSUCHCHANNEL(msg[1]));
        return ;
    }
    //if matching user found, send msg
    Client *cl = serv->GetClient(recipient_fd);
    std::string text = MessageJoin() + "\r\n";
    cl->SendReply(recipient_fd, text);
}

std::string Privmsg::MessageJoin()
{
    std::vector<std::string>::iterator it = this->msg.begin() + 2;
    std::string text;
    
    while (it != msg.end())
        text += *it + " ";
    return text;
}

