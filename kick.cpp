/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 10:49:56 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/06 16:42:29 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

std::vector<std::string> split(const std::string &s, char delimiter);


/* KICK command: is used used to request the forced removal of a user from a channel.
 * 
 * Syntax:          KICK <channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]
 * Numeric Replies: ERR_NEEDMOREPARAMS (461)
 					ERR_NOSUCHCHANNEL (403)
           			ERR_BADCHANMASK (476)    
					ERR_CHANOPRIVSNEEDED (482)
        			ERR_USERNOTINCHANNEL (441)    
					ERR_NOTONCHANNEL (442) 
 */

std::vector<std::string>	ft_split(std::string str, char delim)
{
	std::vector<std::string> lst;
	std::string ch;
	int i = 0;
	
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == delim)) //check other white spaces??
			i++;
		while (str[i] && str[i] != delim)
		{
			ch += str[i];
			i++;
		}
		lst.push_back(ch);
		ch.clear();
		i++;
	}
	return (lst);
}

int is_member(std::map<std::string, Channel *>::iterator it, std::vector<std::string> lst)
{
    for(int i = 0; i < lst.size(); i++)
    {
        if (it->second->isMember(lst.at(i)) == false)
            return (FAILURE);
    }
    return (SUCCESS);
}

void kick(Server *server, int client_fd, msg_struct cmd_infos)
{
	std::vector<std::string> param = ft_split(cmd_infos.parameter, ' ');//param[0]->channel & param[1]->user list
	std::map<std::string, Channel *>ch_lst = server->GetChannelList();
    Client *client = server->GetClient(client_fd);
    std::string kicked_by = client->get_nickname();
    std::vector<std::string> kicked_lst;
    std::string reason;
    int flag;
	std::map<std::string, Channel *>::iterator it_ch = ch_lst.find(param[0]);
    std::vector<std::string> temp;


    for(int i = 0; i < param.size(); i++)
    {
        if (param[i] == ":")
        {
            reason = param[i + 1];
            break;
        }
    }
    temp.push_back(kicked_by);
    if (reason.empty())
        reason = " :Kicked from channel\n";
    flag = param[1].find(',') ? 1 : 0;	// to check if multiple users
	if (flag == 1)
		kicked_lst = ft_split(param[1], ',');
	else
		kicked_lst.push_back(param[1]);

    if (param[0].empty() || kicked_lst.empty())
	{
        server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
		return ;
	}
    else if (it_ch == ch_lst.end())
	{
        server->SendReply(client_fd, ERR_NOSUCHCHANNEL(kicked_by, param[0]));
		return ;
	}
    else if (is_member(it_ch, kicked_lst) == FAILURE)
	{
        server->SendReply(client_fd, ERR_USERNOTINCHANNEL(kicked_by, param[0]));
		return ;
	}
    else if (is_member(it_ch, temp) == FAILURE)
	{
        server->SendReply(client_fd, ERR_NOTONCHANNEL(param[0]));
		return ;
	}
    else if (it_ch->second->isOperator(kicked_by) == false)
	{
        server->SendReply(client_fd, ERR_CHANOPRIVSNEEDED(server->GetServerName(), kicked_by));
		return ;
	}
    else
	{
        //send rpl_kick to all channels
        //erase client from channel list
        //return;
	}

	// for(int i = 0; i < param.size(); i++)
	// 	std::cout << "param: " << i << " " << param.at(i) << "\n";
	// for(int j = 0; ch_flag == 1 && j < channel_lst.size(); j++)
	// 	std::cout << "ch: " << j << channel_lst.at(j) << "\n";
	// for(int j = 0; us_flag ==1 && j < user_lst.size(); j++)
	// 	std::cout << "us: " << j << user_lst.at(j) << "\n";
}