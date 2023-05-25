/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:56 by nali              #+#    #+#             */
/*   Updated: 2023/05/25 14:43:51 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

Mode::Mode()
{}
// mode command format
/* /Mode <channel-name> <modes> <args>*/
Mode::Mode(int client_fd, Server *serv)
{
    Client *c;
    
    this->client_fd = client_fd;
    this->serv = serv;
    c = serv->GetClient(client_fd);
    this->msg = c->message;
    
    if (msg.size() < 3) //if no arguments
    {
        serv->SendReply(client_fd, ERR_NEEDMOREPARAMS(msg[0]));
        return ;
    }
        
    this->chl = serv->GetChannel(msg[1]);
    if (chl == NULL) //channel doesn't exist
    {
        serv->SendReply(client_fd, ERR_NOSUCHCHANNEL(msg[1]));
        return ;
    }

    CheckMode();
}

void Mode::CheckMode()
{
    char ch, prev = '\0';
    std::vector<std::string> options;
    char arr[7] = {'i','t','k','o','l','+','-'};
    bool exists;
    std::string s;
    
    //iterating through mode options
    for (std::string::size_type i = 0; i < msg[2].size(); i++) 
    {
        ch = msg[2][i];
        exists = (std::find(arr, arr + 7, ch) != arr + 7); //checking if mode is valid
        if (exists)
        {
            if (ch == '+' || ch == '-')
                prev = ch;
            else
            {
                if (prev == '+' || prev == '-')
                    s.push_back(prev);
                else
                    s.push_back('+');   //if + or - is not given mode is +
                s.push_back(ch);
                SelectOption(s);
                s.clear();
            }
        }
        else //invalid mode character
        {
            s.push_back(ch); //convert to string
            serv->SendReply(client_fd, ERR_UNKNOWNMODE(s));
            s.clear();
        }
    }
}

Mode::option Mode::hashit (std::string &opt) 
{
    if (opt == "-i") return ONE;
    if (opt == "+i") return TWO;
    if (opt == "-t") return THREE;
    if (opt == "+t") return FOUR;
    if (opt == "-k") return FIVE;
    if (opt == "+k") return SIX;
    if (opt == "-o") return SEVEN;
    if (opt == "+o") return EIGHT;
    if (opt == "-l") return NINE;
    if (opt == "+l") return TEN;
    else return ZERO;
}

void Mode::SelectOption(std::string str)
{
    switch (hashit(str))
    {
        case ZERO:  
        /* code */
        break;
        
        case ONE: /* -i */
            if (this->chl->get_invite_flag() == 1)
            {
                chl->set_invite_flag(0);
                //message channel "<nick> has changed mode: +i"
            }  
        break;
        
        case TWO: /* +i */
            if (this->chl->get_invite_flag() == 0)
            {
                chl->set_invite_flag(1);
                //message channel "<nick> has changed mode: -i"
            }
        break;

        case THREE: /* -t */
            if (this->chl->get_topic_flag() == 1)
            {
                chl->set_topic_flag(0);
                //message channel "<nick> has changed mode: -t"
            }
        break;

        case FOUR:  /* -t */
            if (this->chl->get_topic_flag() == 0)
            {
                chl->set_topic_flag(1);
                //message channel "<nick> has changed mode: +t"
            }
        break;

        case FIVE:  /* -k */
            if (msg[2].size() < 4)
            {
                serv->SendReply(client_fd, ERR_INVALIDMODEPARAM(msg[1], "k", "*", MODEMSG1));
                break;
            }
            if (this->chl->get_key_flag() == 1)
            {
                if (chl->get_key() == msg[3]) //change mode only if key value is correct
                {
                    chl->set_key_flag(0);
                    //message channel "<nick> has changed mode: -k <key value>"
                }
                else
                    serv->SendReply(client_fd, ERR_KEYSET(msg[1]));
                msg.erase(msg.begin() + 3); //deleting that argument
            }
        break;

        case SIX:  /* +k */
            if (msg[2].size() < 4)
            {
                serv->SendReply(client_fd, ERR_INVALIDMODEPARAM(msg[1], "k", "*", MODEMSG1));
                break;
            }
            if (this->chl->get_key_flag() == 0)
            {
                chl->set_key(msg[3]);
                chl->set_key_flag(1);
                //message channel "<nick> has changed mode: +k <key value>"
            }
            msg.erase(msg.begin() + 3); //deleting that argument
        break;

        case SEVEN:  /* -o */
        /* code */
        break;

        case EIGHT:  /* +o */
        /* code */
        break;

        case NINE:  /* -l */
            chl->set_limit_flag(0);
            //message channel "<nick> has changed mode: -l"
        break;

        case TEN:  /* +l */
            if (msg[2].size() < 4)
            {
                serv->SendReply(client_fd, ERR_INVALIDMODEPARAM(msg[1], "l", "*", MODEMSG1));
                break;
            }
            //need not check if it is already set
            int limit = atoi(msg[3].c_str());
            if (limit < 1)
            {
                serv->SendReply(client_fd, ERR_INVALIDMODEPARAM(msg[1], "l", msg[3], MODEMSG2));
                msg.erase(msg.begin() + 3);
                break;
            }  
            chl->set_limit(limit);
            chl->set_limit_flag(1);
            //message channel "<nick> has changed mode: +l <limit value>"
            msg.erase(msg.begin() + 3); //deleting that argument
        break;
    }
}


/*
    https://www.unrealircd.org/docs/Channel_Modes
*/