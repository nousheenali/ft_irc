/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:19:20 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/12 16:52:16 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
void printRcvMsg(int fd, std::string msg);

/* Quit command: is used to terminate a client’s session. The server acknowledges this by sending an ERROR message to the client.
 * 	
 * Syntax: 			QUIT [<Quit message>]
 * Numeric replies:	None
 */

void quit(Server *server, int fd, msg_struct cmd_infos)
{
    Client *c = server->GetClient(fd);
	std::string msg      = cmd_infos.parameter;
    std::string quit_msg = ":" + c->get_nickname() + "!" + c->get_username() + "@localhost QUIT " + msg + "\n";

	server->SendReply(fd, RPL_QUIT(c->get_username(), c->get_nickname(), quit_msg));
    std::cout << RED <<" *** Connection Closed by Client on socket " << fd << " *** \n" << RESET ;
    server->deleteClient(fd);
	server->setPfds(fd);
    return;
}