/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:53 by nali              #+#    #+#             */
/*   Updated: 2023/06/02 09:32:04 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
#define MODE_HPP

#include "Replies.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Server;
class Channel;
class Mode
{
    private:
        int client_fd;
        Client *client;
        Server *serv;
        Channel *chl;
        std::vector<std::string> msg; 
        std::string reply_mode; //reply sent to channel after execution
        std::string reply_args; //reply sent to channel after execution
        enum option{ZERO = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN} ;

    private:
        option hashit (std::string &opt);

    public:
        Mode(int client_fd, Server *serv);
        void SelectOption(std::string str);
        void CheckMode();
};

#endif

/* EXAMPLE 
for command:
/mode #eight k-k+l-l+o-o+t-t+i-i 123 123 5 testuser testuser dummy

the reply sent :
<nick> has changed mode: +k-k+l-l+o-o+t-t+i-i 123 123 5 testuser testuser
*/