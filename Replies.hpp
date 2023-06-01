/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:56:54 by nali              #+#    #+#             */
/*   Updated: 2023/06/01 11:28:28 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

//WELCOME MSG
#define RPL_WELCOME(nickname) ("001" + nickname + ": a Welcome to the Internet Relay Network.\r\n")
#define RPL_YOURHOST(nickname, servername, version) ("002 " + nickname + " :Your host is " + servername + ", running version " + version + "\r\n")
#define RPL_CREATED(client, date) ("003 " + client + " :This server was created " + date + "\r\n")


#define RPL_MYINFO(client, servername, version, nickname, chan_modes, chan_modes_param) ("004 " + client + " " + servername + " " + version + " " + nickname + " " + chan_modes + " " + chan_modes_param + "\r\n")
#define WELCOME2 "hello there"

#define RPL_MYINFO2(str1) (str1 + "\r\n")


# define RPL_ISUPPORT(client, tokens) (":localhost 005 " + client + " " + tokens " :are supported by this server\r\n")

#define ERR_NEEDMOREPARAMS(command)			("461 " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(client)		("462 " + client + " :Unauthorized command (already registered)\r\n")
#define ERR_NONICKNAMEGIVEN()				("431 :No nickname given\r\n")

#define ERR_ERRONEUSNICKNAME(nickname)	("432 " + nickname + " :Erroneous nickname\r\n")

#define ERR_NICKNAMEINUSE(nickname)				("433 " + nickname + " :Nickname is already in use\r\n")
#define ERR_NICKCOLLISION(nickname, user, host)	("436 " + nickname + " :Nickname collision KILL from " + user + "@" + host + "\r\n")
#define ERR_UNAVAILRESOURCE(nick_channel)	("437 " + nick_channel + " :Nick/channel is temporarily unavailable\r\n")
#define ERR_RESTRICTED()					("484 :Your connection is restricted!\r\n")

#define ERR_UNKNOWNCOMMAND(command)	        ("421 " + command + " :Unknown command\r\n")

//mode
#define ERR_NOSUCHCHANNEL(channel)          ("403 " + channel + " :No such channel\r\n")
#define ERR_UNKNOWNMODE(mode)               ("472 " + mode + " :is unknown mode char to me for <channel>\r\n")
#define ERR_INVALIDMODEPARAM(channel, mode, param, msg) ("696 " + channel + " " + mode + " " + param + " " + msg + " Syntax: <key>.\r\n")
#define ERR_KEYSET(channel)                 ("467 " + channel + " :Channel key already set\r\n")
#define ERR_CHANOPRIVSNEEDED(channel)       ("482 " + channel + "You must have channel halfop access or above to set channel mode\r\n")
#define ERR_NOSUCHNICK(nick)                ("401 " + nick +  " :No such nick/channel\r\n")

#define MODEMSG1 "You must specify a parameter for the limit mode."
#define MODEMSG2 "Invalid limit mode parameter."

#define NICK_RPL(oclient, nclient) (":" + oclient + " NICK :" + nclient + "\r\n") //---->check if needed

//PASS
#define ERR_PASSWDMISMATCH(client) ("464 " + client + " :Password incorrect\r\n")


#endif