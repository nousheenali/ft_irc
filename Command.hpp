/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:28:12 by sfathima          #+#    #+#             */
/*   Updated: 2023/06/01 11:27:58 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include "Server.hpp"

# define VALID_LEN 15
class Server;


struct msg_struct
{
	std::string	prefix;
	std::string	cmd;
	std::string	parameter;
};

// class Command
// {
//     private:
//         std::string _cmd;
// 		Client		*_client;
// 		Server		*_server;
		
		
//     public:
//         Command();
//         Command(std::string cmd);
//         virtual ~Command();
		
// 		virtual void execute(std::string cmd)const = 0;  
// };

int		pass(Server *server, int client_fd, msg_struct cmd_infos);
void	nick(Server *server, int fd, msg_struct msg_info);
void	user(Server *server, int client_fd, msg_struct msg_infos);


#endif
