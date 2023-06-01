/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:16:46 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/01 15:45:11 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

/* NICK command: is used to give the client a nickname or change the previous one.
 * 
 * Syntax:          NICK <nickname>
 * Numeric Replies: ERR_NONICKNAMEGIVEN (431)
                    ERR_ERRONEUSNICKNAME (432)
                    ERR_NICKNAMEINUSE (433)
                    ERR_NICKCOLLISION (436)
                    ERR_UNAVAILRESOURCE (437)
                    ERR_RESTRICTED (484) 
 */

static bool check_if_valid(std::string nick)
{
    std::size_t found;
  
    found = nick.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_- ^{}|");

    if (found != std::string::npos)
        return (false);
    // if (nick.size() < 5) //---->change later
    //     return (false);
    return (true);
}

bool already_used(Server *server, int client_fd, std::string new_nickname)
{
	std::map<int, Client *>	client_list	= server->GetAllClients();

	std::map<int, Client *>::iterator	it = client_list.begin();

	while (it != client_list.end())
	{
		if (it->second->get_socket() != client_fd && it->second->get_nickname() == new_nickname)
			return (true);
		it++;
	}
	return (false);
}

void nick(Server *server, int fd, msg_struct msg_info)
{
    std::string nickname = msg_info.parameter;
    Client *c = server->GetClient(fd);

    if (c->isRegistration() == false)
	{
		c->set_nickname(nickname);
		c->set_count(1);
	}

    if (nickname.empty())   // no nickname given
    {
		server->SendReply(fd, ERR_NONICKNAMEGIVEN());
		if (c->isRegistration() == false)
			c->set_count(0);
	}
    else if (check_if_valid(nickname) == false) //check valid characters and length is 8/9
    {
		server->SendReply(fd, ERR_ERRONEUSNICKNAME(nickname));
		if (c->isRegistration() == false)
			c->set_count(0);
	}
    else if (already_used(server, fd, nickname) == true) //check if nickname given is already used by available clients
    {
		server->SendReply(fd, ERR_NICKNAMEINUSE(nickname));
		if (c->isRegistration() == false)
			c->set_count(0);
	}
    else 
    {	
		if (c->isRegistration() == true)
		{
            c->set_old_nickname(c->get_nickname());
			std::cout << "[Server] Nickname change registered. Old nickname is now : " << c->get_old_nickname() << std::endl;
			c->set_nickname(nickname);
		}
	}
}
