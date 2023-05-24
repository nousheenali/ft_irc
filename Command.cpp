/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:27:49 by sfathima          #+#    #+#             */
/*   Updated: 2023/05/24 14:57:30 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command()	{}

Command::~Command()	{}

Command::Command(std::string cmd)
{
	this->_cmd = cmd;
}