// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   mode.hpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/05/24 11:32:53 by nali              #+#    #+#             */
// /*   Updated: 2023/06/03 22:42:21 by nali             ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #ifndef MODE_HPP
// #define MODE_HPP

// #include "Replies.hpp"
// #include "Channel.hpp"
// #include "Server.hpp"

// class Server;
// std::vector<std::string>  convert_to_vector(std::string msg);

// class mode
// {
//     private:
//         int client_fd;
//         Client *client;
//         Server *serv;
//         Channel *chl;
//         std::vector<std::string> params; 
//         std::string reply_mode; //reply sent to channel after execution
//         std::string reply_args; //reply sent to channel after execution
//         enum option{ZERO = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN} ;

//     private:
//         option hashit (std::string &opt);

//     public:
//         mode(Server *serv, int client_fd, msg_struct msg_info);
//         void SelectOption(std::string str);
//         void CheckMode();
// };

// #endif

// /* EXAMPLE 
// for command:
// /mode #eight k-k+l-l+o-o+t-t+i-i 123 123 5 testuser testuser dummy

// the reply sent :
// <nick> has changed mode: +k-k+l-l+o-o+t-t+i-i 123 123 5 testuser testuser
// */