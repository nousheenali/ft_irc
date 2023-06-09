/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 10:49:56 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/15 22:57:41 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

std::vector<std::string> split(const std::string &s, char delimiter);
std::vector<std::string> convert_to_vector(std::string msg);
std::string ParamsJoin(std::vector<std::string> vec);


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
	std::vector<std::string> vec;
    std::string tmp = "\0";

    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] != delim)
            tmp.push_back(str[i]);
        else
        {
            vec.push_back(tmp);
            tmp.clear();
        }
    }
    if (tmp != "\0")
        vec.push_back(tmp);
    return vec;
}

int is_member(std::map<std::string, Channel *>::iterator it, std::vector<std::string> lst)
{
    for(size_t i = 0; i < lst.size(); i++)
    {
        if (it->second->isMember(lst.at(i)) == false)
            return (FAILURE);
    }
    return (SUCCESS);
}

void kick(Server *server, int client_fd, msg_struct cmd_infos)
{
    Client *client					= server->GetClient(client_fd);
	std::vector<std::string> param 	= convert_to_vector(cmd_infos.parameter);//param[0]->channel & param[1]->user list & param[2]->reason
  	if (param.empty())
    {		
		server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
		return ;
	}
	
	std::map<std::string, Channel *>ch_lst	= server->GetChannelList();
	std::map<std::string, Channel *>::iterator it_ch = ch_lst.find(param[0]);
    std::vector<std::string> kicked_lst;
    std::vector<std::string> temp;
    std::string kicked_by = client->get_nickname();
    std::string reason = "";
    temp.push_back(kicked_by);
    
	if (param.size() > 2)
	{
		//if mutliple words are present in reason, we will need ':' at the beginning.
		//  but if there are tabs in the message ':' is not added automatically
		if (param.size() > 3 && param[2][0] != ':') 
			reason = ":";
		for (std::vector<std::string>::iterator it = param.begin() + 2; it != param.end(); it++)
			reason += (*it + " ");
	}
	else
		reason = " :byeee!";
	// if (!cmd_infos.parameter.empty())
	// {
	// 	if (cmd_infos.parameter.find(':') != std::string::npos)
	// 		reason = cmd_infos.parameter.substr(cmd_infos.parameter.find(':'), cmd_infos.parameter.length());
	// 	else if (param.size() > 1)
	// 		reason = param[2];
	// }

	//check for multiple users to be kicked out
	int flag = 0;
	if (param.size() > 1)
	{
    	flag = param[1].find(',') ? 1 : 0;
		if (flag == 1)
			kicked_lst = ft_split(param[1], ',');
		else
			kicked_lst.push_back(param[1]);
	}

	//error handling
    if ((param[0].find(':') != std::string::npos) || kicked_lst.empty())
		server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
    else if (it_ch == ch_lst.end())
        server->SendReply(client_fd, ERR_NOSUCHCHANNEL(kicked_by, param[0]));
    else if (is_member(it_ch, kicked_lst) == FAILURE)
        server->SendReply(client_fd, ERR_USERNOTINCHANNEL(kicked_by, kicked_lst[0], param[0])); //change to kicked_lst
    else if (is_member(it_ch, temp) == FAILURE)
        server->SendReply(client_fd, ERR_NOTONCHANNEL(param[0]));
    else if (it_ch->second->isOperator(kicked_by) == false)
        server->SendReply(client_fd, ERR_CHANOPRIVSNEEDED(server->GetServerName(), kicked_by));
    else
	{
        //send rpl_kick to all channels
		std::vector<struct Channel::Channel_Member> it = it_ch->second->getClients();
        std::vector<struct Channel::Channel_Member>::iterator ite = it.begin();
        while(ite != it.end())
        {
			for (size_t i = 0; i < kicked_lst.size(); i++)
    			server->SendReply(ite->user->get_socket(), RPL_KICK(client->get_msg_prefix(), it_ch->second->get_channel_name(), kicked_lst[i], reason));
    		ite++;
        }
		for (size_t i = 0; i < kicked_lst.size(); i++)
		{
			ite = it.begin();
			it_ch->second->removeUser(kicked_lst[i]);
			while(ite != it.end())
			{
				if (ite->user->get_nickname() == kicked_lst[i])
				{
					it_ch->second->removeOperator(ite->user);
					break;
				}
				ite++;
			}
		}
	}
}