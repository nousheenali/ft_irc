/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:20:30 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/01 11:20:35 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

/* USER command: is used at the beginning of a connection to specify the username, hostname and realname of a new user
 * 	
 * Syntax: USER <username> 0 * <realname>
 * Numeric Replies:    ERR_NEEDMOREPARAMS (461)
                        ERR_ALREADYREGISTERED (462)
 */

std::string	findUsername(std::string msg)
{
	std::string user;
	user.clear();

	if (msg.empty() == false && msg[0] == ' ')
		msg.erase(0, 1);
	if (msg.empty() == false)
	{
		if (msg.find_first_of(' ') != std::string::npos)
			user.insert(0, msg, 0, msg.find(' '));
		else
			user.insert(0, msg, 0, std::string::npos);
	}
	return (user);
}

std::string	findRealname(std::string msg)
{
	std::string real;
	real.clear();

	if (msg.empty() == false && msg[0] == ' ')
		msg.erase(0, 1);
	if (msg.empty())
		return (real);
	
	int space_count = 0;
	int	begin_pos = 0;
	for (size_t i = 0; i < msg.size(); i++)
	{
		if (msg[i] == ' ')
			space_count++;
		if (space_count == 3)
		{
			begin_pos = i;
			break;
		}
	}
	if (space_count >= 3)
		real.insert(0, msg, begin_pos + 1, std::string::npos); 
	if (real[0] == ':')
		real.erase(0, 1);
	return (real);
}


void	user(Server *server, int client_fd, msg_struct msg_infos)
{
	Client*		client		= server->GetClient(client_fd);
	std::string	username	= findUsername(msg_infos.parameter);
	std::string realname	= findRealname(msg_infos.parameter);

	if (username.empty() || realname.empty())
		server->SendReply(client_fd, ERR_NEEDMOREPARAMS(msg_infos.cmd));
	else if (client->isRegistration() == true)
		server->SendReply(client_fd, ERR_ALREADYREGISTRED(client->get_nickname()));
	else
	{
		client->set_username(username);
		client->set_realname(realname);
		client->set_count(1);
	}
}