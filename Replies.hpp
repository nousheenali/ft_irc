/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:56:54 by nali              #+#    #+#             */
/*   Updated: 2023/05/25 14:32:28 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP


#define RPL_WELCOME(nick) ("001" + nick + ": a Welcome to the Internet Relay Network.\r\n")

#define ERR_NEEDMOREPARAMS(command)			("461 " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED()				("462 :Unauthorized command (already registered)\r\n")
#define ERR_NONICKNAMEGIVEN()				("431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick)			("432 " + nick + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick)				("433 " + nick + " :Nickname is already in use\r\n")
#define ERR_NICKCOLLISION(nick, user, host)	("436 " + nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")
#define ERR_UNAVAILRESOURCE(nick_channel)	("437 " + nick_channel + " :Nick/channel is temporarily unavailable\r\n")
#define ERR_RESTRICTED()					("484 :Your connection is restricted!\r\n")

#define ERR_UNKNOWNCOMMAND(command)	        ("421 " + command + " :Unknown command\r\n")

//mode
#define ERR_NOSUCHCHANNEL(channel)          ("403 " + channel + " :No such channel\r\n")
#define ERR_UNKNOWNMODE(mode)               ("472 " + mode + " :is unknown mode char to me for <channel>\r\n")
#define ERR_INVALIDMODEPARAM(channel, mode, param, msg) ("696 " + channel + " " + mode + " " + param + " " + msg + " Syntax: <key>.\r\n")
#define ERR_KEYSET(channel)                 ("467 " + channel + " :Channel key already set\r\n")


#define MODEMSG1 "You must specify a parameter for the limit mode."
#define MODEMSG2 "Invalid limit mode parameter."

#endif