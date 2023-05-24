/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:53 by nali              #+#    #+#             */
/*   Updated: 2023/05/24 11:34:32 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
#define MODE_HPP

#include "Replies.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Server;
class Mode
{
    // enum option{ZERO = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN} ;
    // option hashit (std::string &opt);

    public:
        Mode();
        Mode(int client_fd, Server *serv);
};

#endif