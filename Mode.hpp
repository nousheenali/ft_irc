/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:53 by nali              #+#    #+#             */
/*   Updated: 2023/05/25 13:36:09 by nali             ###   ########.fr       */
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
        Server *serv;
        Channel *chl;
        std::vector<std::string> msg;
        enum option{ZERO = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN} ;

    private:
        option hashit (std::string &opt);

    public:
        Mode();
        Mode(int client_fd, Server *serv);
        void SelectOption(std::string str);
        void CheckMode();
};

#endif