/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:28:12 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/04 14:00:36 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include "Server.hpp"
#include "Channel.hpp"

#define VALID_LEN 15
class Server;

struct msg_struct
{
	std::string prefix;
	std::string cmd;
	std::string parameter;
};

int  pass(Server *server, int clifdent_fd, msg_struct msg_info);
void nick(Server *server, int fd, msg_struct msg_info);
void user(Server *server, int client_fd, msg_struct msg_info);
void quit(Server *server, int fd, msg_struct msg_info);
void pong(Server *server, int client_fd, msg_struct msg_info);
int  join(Server *server, int client_fd, msg_struct msg_info);
void kick(Server *server, int client_fd, msg_struct msg_info);


#endif