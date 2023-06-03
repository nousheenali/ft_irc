/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:18:35 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/04 00:35:46 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

static void  splitMsg(std::vector<std::string> &cmds, std::string msg)
{
	int pos = 0;
	std::string substr;

	while ((pos = msg.find("\n")) != static_cast<int>(std::string::npos))
	{
		substr = msg.substr(0, pos);
		cmds.push_back(substr);
		msg.erase(0, pos + 1);
	}
}

int	parseCommand(std::string cmd_line, msg_struct &cmd_infos)
{
	if (cmd_line.empty() == true)
		return (FAILURE);
	
	std::string copy = cmd_line;
	if (cmd_line[0] == ':')	// if prefix is : delete until first space
	{
		if (cmd_line.find_first_of(' ') != std::string::npos)
			copy.erase(0, copy.find_first_of(' ') + 1);
	}
	
	if (copy.find_first_of(' ') == std::string::npos) // incase of command without arguments
	{
		cmd_infos.cmd.insert(0, copy, 0, std::string::npos);
		if (cmd_infos.cmd.find('\r') != std::string::npos) // delete the \r\n
			cmd_infos.cmd.erase(cmd_infos.cmd.find('\r'), 1);
	}
	else
		cmd_infos.cmd.insert(0, copy, 0, copy.find_first_of(' ')); //---->put the command inside struct

	size_t prefix_length = cmd_line.find(cmd_infos.cmd, 0);
	cmd_infos.prefix.assign(cmd_line, 0, prefix_length);//---->put prefix into struct
	size_t msg_beginning = cmd_line.find(cmd_infos.cmd, 0) + cmd_infos.cmd.length() + 1;
	if (msg_beginning < cmd_line.length())
		cmd_infos.parameter = cmd_line.substr(msg_beginning, std::string::npos);//---->put pararmeter into struct
	if (cmd_infos.parameter.find("\r") != std::string::npos)
		cmd_infos.parameter.erase(cmd_infos.parameter.find("\r"), 1);//---->remove \r\n

	for (size_t i = 0; i < cmd_infos.cmd.size(); i++)
		cmd_infos.cmd[i] = std::toupper(cmd_infos.cmd[i]);
	
	// std::cout << "Command : " << cmd_infos.cmd << "\n";
	// std::cout << "Prefix : " << cmd_infos.prefix << "\n";
	// std::cout << "Message : " << cmd_infos.parameter << "\n";
	return (SUCCESS);
}

void Server::fillDetails(Client *c , int client_fd, std::string cmd)
{
	msg_struct msg_info;
	int temp;

	if (parseCommand(cmd, msg_info) == FAILURE)
		return ;
	temp =  (cmd.find(' ') == std::string::npos) ? 0 : cmd.find_first_of(' ');

	for (size_t i = 0; i < temp; i++)
		cmd[i] = std::toupper(cmd[i]);
		
	if (cmd.find("NICK ") != std::string::npos)
		nick(this, client_fd, msg_info);
	else if (cmd.find("USER ") != std::string::npos)
		user(this, client_fd, msg_info);
	else if (cmd.find("PASS ") != std::string::npos)
	{
		if (pass(this, client_fd, msg_info) == SUCCESS)
			c->set_passFlag(true);
		else
			c->set_passFlag(false);
	}
}

void printRcvMsg(int fd, std::string msg)
{
	std::cout << "[Server] Message sent to client " << fd << ": " << RED << msg << RESET << "\n";
}

void Server::parseMessage(int fd, std::string msg)
{
    std::vector<std::string> cmds;
    Client *c = GetClient(fd);

	splitMsg(cmds, msg);

	for (size_t i = 0; i != cmds.size(); i++)
	{
		if (c->isAuthDone() == false)
		{
			if (c->has_all_info() == false)
			{
				fillDetails(c, fd, cmds[i]);
				if (!c->get_nickname().empty() && !c->get_username().empty() && c->get_passFlag())
					c->has_all_info() = true;
			}
			if (c->has_all_info() == true && c->first_invite() == false)
			{
		        this->SendReply(fd, RPL_WELCOME(this->GetServerName(), c->get_nickname()));
				// printRcvMsg(fd, RPL_WELCOME(c->get_nickname()));
                this->SendReply(fd, RPL_YOURHOST(c->get_nickname(), this->GetServerName(), "1.1")); //--->get version and other details
				// printRcvMsg(fd, RPL_YOURHOST(c->get_nickname(), "ft_irc", "1.1"));
				this->SendReply(fd, RPL_CREATED(this->GetServerName(), c->get_nickname(), this->getDate())); //---->get date in realtime and print it
				// printRcvMsg(fd, RPL_CREATED(c->get_nickname(), "31 May 2023"));
                this->SendReply(fd, RPL_MYINFO(c->get_nickname(), this->GetServerName(), "1.1", c->get_nickname(), "channel_modes", "channel_modes_parameters"));
				// printRcvMsg(fd, RPL_MYINFO(c->get_nickname(), "localhost", "1.1", c->get_nickname(), "channel_modes", "channel_modes_parameters"));
				printRcvMsg(fd, ": Welcome message sent...\n");
				
				c->first_invite() = true;
                c->isAuthDone() = true;
			}
		}
		else
			execCommand(fd, cmds[i]);
	}
}

void Server::execCommand(int client_fd, std::string cmd_line)
{
	msg_struct 	cmd_infos;
	Client 		*client		  = this->GetClient(client_fd);
	int i = 0;

	std::string	validCmds[10] = {"NICK", "USER", "QUIT", "PASS", "PRIVMSG", "MODE"}; //---->keep adding commands

	if (parseCommand(cmd_line, cmd_infos) == FAILURE)
		return ;
	while (i < 10)
	{
		if (cmd_infos.cmd == validCmds[i])
			break;
		i++;
	}
	switch (i)
	{
		case 0: nick(this, client_fd, cmd_infos);	break;
		case 1: user(this, client_fd, cmd_infos);	break;
		case 2: quit(this, client_fd, cmd_infos);	break;
		case 3: pass(this, client_fd, cmd_infos);	break;
		case 4: privmsg(this, client_fd, cmd_infos);	break;
		case 5: mode(this, client_fd, cmd_infos);	break;
		default:
			this->SendReply(client_fd, ERR_UNKNOWNCOMMAND(cmd_infos.cmd));
	}
}