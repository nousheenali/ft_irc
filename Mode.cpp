/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:56 by nali              #+#    #+#             */
/*   Updated: 2023/06/07 14:08:27 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"
#include <algorithm>

// mode command format
/* /mode <channel-name> <modes> <args>*/

mode::mode(Server *serv, int client_fd, msg_struct msg_info)
{
    this->client_fd = client_fd;
    this->serv = serv;
    this->client = serv->GetClient(client_fd);
    this->params = convert_to_vector(msg_info.parameter);
    this->modeChanged = false;

    if (params.size() == 0) // if no arguments
    {
        serv->SendReply(client_fd, ERR_NEEDMOREPARAMS(msg_info.cmd));
        return;
    }
    this->chl = serv->GetChannel(params[0]);
    if (chl == NULL) // channel doesn't exist
    {
        serv->SendReply(client_fd, ERR_NOSUCHCHANNEL(client->get_nickname(), params[0]));
        return;
    }
    if (params.size() == 1) // in case of "/mode <#channle_name>"
    {
        serv->SendReply(client_fd, RPL_CHANNELMODEIS(serv->GetServerName(), client->get_nickname(), chl->get_channel_name(), chl->getChannelMode()));
        return;
    }
    CheckMode();
}

void mode::CheckMode()
{
    char ch, prev = '\0';
    std::vector<std::string> options;
    char arr[7] = {'i', 't', 'k', 'o', 'l', '+', '-'};
    bool exists;
    std::string s;

    // iterating through mode options
    for (std::string::size_type i = 0; i < params[1].size(); i++)
    {
        ch = params[1][i];
        exists = (std::find(arr, arr + 7, ch) != arr + 7); // checking if mode is valid
        if (exists)
        {
            if (ch == '+' || ch == '-')
                prev = ch;
            else
            {
                if (prev == '+' || prev == '-')
                    s.push_back(prev);
                else
                    s.push_back('+'); // if + or - is not given mode is +

                s.push_back(ch);
                SelectOption(s);
                s.clear();
                prev = '\0';
            }
        }
        else // invalid mode character
        {
            s.push_back(ch); // convert to string
            serv->SendReply(client_fd, ERR_UNKNOWNMODE(serv->GetServerName(), client->get_nickname(), s));
            s.clear();
        }
    }
    // serv->SendReply(client_fd, RPL_CHANNELMODEIS(cl->get_msg_prefix(),chl->get_channel_name(), "mode changed"));
    if (modeChanged)
    {
        std::string str = reply_mode + " " + reply_args;
        std::map<int, Client *> mp = serv->GetAllClients();

        for (std::map<int, Client *>::iterator it = mp.begin(); it != mp.end(); it++)
        {
            if (chl->isMember(it->second->get_nickname()))
                serv->SendReply(it->first, RPL_MODE(client->get_msg_prefix(), chl->get_channel_name(), str));
            // serv->SendReply(it->first, RPL_CHANNELMODEIS2(client->get_msg_prefix(), client->get_nickname(), chl->get_channel_name(), str));
        }
    }
}

// using a hash function becauce switches dont handle strings
mode::option mode::hashit(std::string &opt)
{
    if (opt == "-i")
        return ONE;
    if (opt == "+i")
        return TWO;
    if (opt == "-t")
        return THREE;
    if (opt == "+t")
        return FOUR;
    if (opt == "-k")
        return FIVE;
    if (opt == "+k")
        return SIX;
    if (opt == "-o")
        return SEVEN;
    if (opt == "+o")
        return EIGHT;
    if (opt == "-l")
        return NINE;
    if (opt == "+l")
        return TEN;
    else
        return ZERO;
}

void mode::SelectOption(std::string str)
{
    // proceed to excute mode only if the user has operator rights
    if (!chl->isOperator(client->get_nickname()))
    {
        serv->SendReply(client_fd, ERR_CHANOPRIVSNEEDED(serv->GetServerName(), client->get_nickname()));
        return;
    }
    switch (hashit(str))
    {
    case ZERO:
        break;

    case ONE: /* -i */
        if (this->chl->get_invite_flag() == 1)
        {
            chl->set_invite_flag(0);
            reply_mode += "-i";
            modeChanged = true;
        }
        break;

    case TWO: /* +i */
        if (this->chl->get_invite_flag() == 0)
        {
            chl->set_invite_flag(1);
            reply_mode += "+i";
            modeChanged = true;
        }
        break;

    case THREE: /* -t */
        if (this->chl->get_topic_flag() == 1)
        {
            chl->set_topic_flag(0);
            reply_mode += "-t";
            modeChanged = true;
        }
        break;

    case FOUR: /* -t */
        if (this->chl->get_topic_flag() == 0)
        {
            chl->set_topic_flag(1);
            reply_mode += "-t";
            modeChanged = true;
        }
        break;

    case FIVE: /* -k */
        if (CheckParams('k'))
            break;
        if (this->chl->get_key_flag() == 1) // mode is changed to -k only if current flag +k
        {
            if (chl->get_key() == params[2]) // change mode only if key value is correct
            {
                chl->set_key_flag(0);
                reply_mode += "-k";
                reply_args += (params[2] + " ");
                modeChanged = true;
            }
            else
                serv->SendReply(client_fd, ERR_KEYSET(serv->GetServerName(), client->get_nickname(), params[0]));
            params.erase(params.begin() + 2); // deleting the argument for k mode so we move to argument of next mode
        }
        break;

    case SIX: /* +k */
        if (CheckParams('k'))
            break;
        if (this->chl->get_key_flag() == 0)
        {
            chl->set_key(params[2]);
            chl->set_key_flag(1);
            reply_mode += "+k";
            reply_args += (params[2] + " ");
            modeChanged = true;
        }
        params.erase(params.begin() + 2); // deleting that argument
        break;

    case SEVEN: /* -o */
        if (CheckParams('o'))
            break;
        if (chl->isMember(params[2])) // check if user is member
        {
            if (chl->isOperator(params[2])) // if an operator
            {
                chl->removeOperator(serv->GetClient(params[2]));
                reply_mode += "-o";
                reply_args += (params[2] + " ");
                modeChanged = true;
            }
        }
        break;

    case EIGHT: /* +o */
        if (CheckParams('o'))
            break;
        if (chl->isMember(params[2])) // check if user is a member
        {
            if (!chl->isOperator(params[2])) // if not an operator already
            {
                chl->addOperator(serv->GetClient(params[2]));
                reply_mode += "+o";
                reply_args += (params[2] + " ");
                modeChanged = true;
            }
        }
        break;
    case NINE: /* -l */
        if (this->chl->get_limit_flag() == 1)
        {
            chl->set_limit_flag(0);
            reply_mode += "-l";
            modeChanged = true;
        }
        break;

    case TEN: /* +l */
        if (CheckParams('l') == 1)
            break;
        // need not check if limit is already set
        int limit = atoi(params[2].c_str());
        if (limit < 1)
        {
            serv->SendReply(client_fd, ERR_INVALIDMODEPARAM2(serv->GetServerName(), params[0], client->get_nickname(), "l", params[2]));
            params.erase(params.begin() + 2);
            break;
        }
        chl->set_limit(limit);
        chl->set_limit_flag(1);
        reply_mode += "+l";
        reply_args += (params[2] + " ");
        params.erase(params.begin() + 2); // deleting that argument
        break;
    }
}

int mode::CheckParams(char c)
{
    if (params.size() < 3) // these mode require an argument to execute
    {
        serv->SendReply(client_fd, ERR_INVALIDMODEPARAM(serv->GetServerName(), params[0], client->get_nickname(), c));
        return (1);
    }
    return (0);
}

/*
    https://www.unrealircd.org/docs/Channel_Modes
*/
