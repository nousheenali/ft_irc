/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:28:12 by sfathima          #+#    #+#             */
/*   Updated: 2023/05/24 14:57:25 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
#include "Server.hpp"

class Command
{
    private:
        std::string _cmd;
		Client		*_client;
		Server		*_server;
		
		
    public:
        Command();
        Command(std::string cmd);
        virtual ~Command();
		
		virtual void execute(std::string cmd)const = 0;  
};
