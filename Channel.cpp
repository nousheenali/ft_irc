/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:26:23 by nali              #+#    #+#             */
/*   Updated: 2023/06/06 10:38:20 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
// #include "Client.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string name)
{
    this->name = name;
    iflag = 0;
    tflag = 1;
    kflag = 0;
    lflag = 0;
    this->total_members = 1;
}

bool Channel::isMember(std::string nick)
{
    for (long unsigned int i = 0; i < this->members.size(); i++)
    {
        if (this->members[i].user->get_nickname() == nick)
            return (true);
    }
    return (false);
}

bool Channel::isFull()
{
    return this->total_members >= limit;
}

int Channel::get_total_members()
{
    return (total_members);
}

bool Channel::get_invite_flag()
{
    return iflag;
}

void Channel::set_invite_flag(bool i)
{
    iflag = i;
}

bool Channel::get_topic_flag()
{
    return tflag;
}

void Channel::set_topic_flag(bool i)
{
    tflag = i;
}

bool Channel::get_key_flag()
{
    return kflag;
}

void Channel::set_key_flag(bool i)
{
    kflag = i;
}

bool Channel::get_limit_flag()
{
    return lflag;
}

void Channel::set_limit_flag(bool i)
{
    lflag = i;
}

std::string Channel::get_key()
{
    return this->key;
}

void Channel::set_key(std::string key)
{
    this->key = key;
}

int Channel::get_limit()
{
    return this->limit;
}

void Channel::set_limit(int val)
{
    this->limit = val;
}

std::string Channel::get_topic()
{
    return this->topic;
}

void Channel::set_topic(std::string str)
{
    this->topic = str;
}
