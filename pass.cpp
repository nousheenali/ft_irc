/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:19:00 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/01 16:38:44 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"


/* PASS command: is used to set a ‘connection password’. It must match the one defined in the server.
 * 	
 * Syntax: 			PASS <password>
 * Numeric replies:	ERR_NEEDMOREPARAMS (461)
 					ERR_ALREADYREGISTRED (462)
 					ERR_PASSWDMISMATCH (464)
 */

std::string	retrievePassword(std::string msg_to_parse)
{
	std::string	password;
	size_t i = 0;
	
	password.clear();
	
	while (msg_to_parse[i] && msg_to_parse[i] == ' ')
		i++;
	while (msg_to_parse[i] && msg_to_parse[i] != ' ')
		password += msg_to_parse[i++];
	return (password);
}

int		pass(Server *server, int client_fd, msg_struct cmd_infos)
{
	Client*	client = server->GetClient(client_fd);
	std::string	password	= retrievePassword(cmd_infos.parameter);

	if (cmd_infos.parameter.empty() == true || password.empty() == true)
	{
		server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
		return (FAILURE);
	}
	else if (server->getPassword() != password && client->get_passFlag() == false)
	{
		// std::cout << password << "*" << server->getPassword() << "\n";
		server->SendReply(client_fd, ERR_PASSWDMISMATCH(cmd_infos.cmd));
		password.clear();
		return (FAILURE);
	}
	else if (client->get_passFlag() == true)
		server->SendReply(client_fd, ERR_ALREADYREGISTRED(client->get_nickname()));

	else
		return (SUCCESS);
	return (SUCCESS);
}
