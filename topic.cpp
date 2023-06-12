/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:56:34 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/12 15:04:32 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

std::vector<std::string> convert_to_vector(std::string msg);
std::vector<std::string> split(const std::string &s, char delimiter);
void set_topic(std::vector<std::string> vec, Channel *ch, Client *cl, Server *s);


int topic(Server *server, int client_fd, msg_struct cmd_infos)
{
	std::string str;
    std::vector<std::string> param_splitted = convert_to_vector(cmd_infos.parameter);
    Client *cl = server->GetClient(client_fd);

    if (param_splitted.size() < 1)
    {
        server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
        return (FAILURE);
    }

    std::string channelName = param_splitted[0];
    std::map<std::string, Channel *>::iterator it = server->GetChannelList().find(channelName);
    if (it == server->GetChannelList().end())
    {
        server->SendReply(client_fd, ERR_NOSUCHCHANNEL(cl->get_nickname(), channelName));
        return (FAILURE);
    }

    Channel *ch = it->second;
    if (!ch->isMember(cl->get_nickname()))
    {
        server->SendReply(client_fd, ERR_NOTONCHANNEL(ch->get_channel_name()));
        return (FAILURE);
    }

    // If 2 parameters are given, the client wants to change the topic
    if (param_splitted.size() >= 2)
    {
        // Check for the operator privileges
        if (!ch->isOperator(cl->get_nickname()))
        {
            server->SendReply(client_fd, ERR_CHANOPRIVSNEEDED(server->getServerIP(), cl->get_nickname()));
            return (FAILURE);
        }

        // Set the new topic
		set_topic(param_splitted, ch, cl, server);
    }
    else // The client wants to know the current topic
    {
        if (ch->get_topic().empty())
        {
            std::cout << "Topic of the channel : "  << GREEN << "No topic set" << RESET << "\n";
            server->SendReply(client_fd, RPL_NOTOPIC(server->GetServerName(), cl->get_nickname(), ch->get_channel_name(), "no topic set"));
        }
        else
        {
            std::cout << "Topic of the channel : "  << GREEN << ch->get_topic() << RESET << "\n";
            server->SendReply(client_fd, RPL_TOPIC(server->GetServerName(), cl->get_nickname(), ch->get_channel_name(), ch->get_topic()));
        }
    }
    return (SUCCESS);
}

void set_topic(std::vector<std::string> param_splitted, Channel *ch, Client *cl, Server *server)
{
	std::string str;

	if (param_splitted.size() == 2)
	{
		ch->set_topic(param_splitted[1]);
		std::vector<struct Channel::Channel_Member> it = ch->getClients();
       	std::vector<struct Channel::Channel_Member>::iterator ite = it.begin();
       	while(ite != it.end())
       	{
    		server->SendReply(ite->user->get_socket(), RPL_TOPIC1(cl->get_msg_prefix(), ch->get_channel_name(), ch->get_topic()));
    		ite++;
       	}
	}
	else
	{
		if (param_splitted[1][0] == ':')
		{
			str = param_splitted[1].substr(1) + " ";
			for (size_t i = 2; i < param_splitted.size(); i++)
				str += param_splitted[i] + " ";
		}
		ch->set_topic(str);
		std::vector<struct Channel::Channel_Member> it = ch->getClients();
       	std::vector<struct Channel::Channel_Member>::iterator ite = it.begin();
       	while(ite != it.end())
       	{
    		server->SendReply(ite->user->get_socket(), RPL_TOPIC1(cl->get_msg_prefix(), ch->get_channel_name(), ":" + ch->get_topic()));
    		ite++;
       	}
		str.clear();
	}
}
