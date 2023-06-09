/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:26:29 by nali              #+#    #+#             */
/*   Updated: 2023/06/09 10:26:30 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include <cstdio>
void printRcvMsg(int fd, std::string msg);

/* list command: To list channel in the server
 * Syntax: 			LIST
 * Numeric replies:	None
 */

void list(Server *server, int fd, msg_struct cmd_infos)
{
    Client *c = server->GetClient(fd);
	std::string msg      = cmd_infos.parameter;
    char members[20];

    std::map<std::string, Channel *>  chlist = server->GetChannelList();
    std::map<std::string, Channel *>::iterator it;

	server->SendReply(fd, RPL_MYINFO2(c->get_nickname(), server->GetServerName(), "List of Channels: "));
    std::cout << " --- channel list ---"<< std::endl;
    for (it = chlist.begin(); it != chlist.end(); it++)
    {
        sprintf(members, "%d", it->second->get_total_members()); //convert string to int
        std::string str = ":" + server->GetServerName() + " 322 " + c->get_nickname() + " " +
        it->first + " " + members + " " + it->second->get_topic() + "\r\n";	
        server->SendReply(fd, str);
        std::cout << " - " << it->first << std::endl;
    }
    return;
}