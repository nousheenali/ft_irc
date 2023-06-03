/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:19:20 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/04 00:13:13 by nali             ###   ########.fr       */
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

    printRcvMsg(fd, quit_msg);
    std::cout << RED <<" *** Connection Closed by Client on socket " << fd << " *** \n" << RESET ;
    server->deleteClient(fd);
	server->setPfds(fd, -1);
    return;
}