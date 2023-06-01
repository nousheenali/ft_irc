/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:19:20 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/01 13:49:29 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

/* Quit command: is used to terminate a client’s session. The server acknowledges this by sending an ERROR message to the client.
 * 	
 * Syntax: 			QUIT [<Quit message>]
 * Numeric replies:	None
 */

void quit(Server *server, int fd, msg_struct cmd_infos)
{
	std::cout << RED <<" *** Connection Closed by Client *** \n" << RESET ;
	
    server->deleteClient(fd);
	server->setPfds(fd, -1);
	// server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));

        // pfds[i].fd = -1; //make the socket fd negative so it is ignored in future
        return;
}