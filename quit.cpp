/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:19:20 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/11 10:46:46 by nali             ###   ########.fr       */
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

    removeUserFromChannels(server, fd);
    printRcvMsg(fd, quit_msg);
    std::cout << RED <<" *** Connection Closed by Client on socket " << fd << " *** \n" << RESET ;
    server->deleteClient(fd);
	server->setPfds(fd);
    return;
}

void removeUserFromChannels(Server *server, int fd)
{
    Client *c = server->GetClient(fd);
    std::map<std::string, Channel *> chllist = server->GetChannelList();

    for (std::map<std::string, Channel *>::iterator it = chllist.begin(); it != chllist.end(); it++ )
    {
        if (it->second->isMember(c->get_nickname()))
        {
            it->second->removeUser(c->get_nickname());
            if (it->second->get_total_members() == 0 && it->second->get_channel_name()[0] != '!') //dont destroy safe channels starting with '!'
                server->RemoveChannel(it->second->get_channel_name());
        }
    }
}