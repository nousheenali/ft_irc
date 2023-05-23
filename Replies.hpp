/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:56:54 by nali              #+#    #+#             */
/*   Updated: 2023/05/22 19:35:17 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#define RPL_WELCOME(nick) ("001" + nick + ": a Welcome to the Internet Relay Network.\r\n")
#define ERR_UNKNOWNCOMMAND(nick, command)	 ("421 " + nick + " " + command + " :Unknown command\r\n")

#endif