/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:28:12 by sfathima          #+#    #+#             */
/*   Updated: 2023/05/25 14:42:17 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <string>
#include "Server.hpp"

class Server;
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
