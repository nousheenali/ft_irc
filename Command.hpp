/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfathima <sfathima@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:28:12 by sfathima          #+#    #+#             */
/*   Updated: 2023/05/23 13:35:19 by sfathima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once COMMAND_HPP

#include <string>

class Command
{
    private:
        std::string cmd;
		
    public:
        Command();
        Command(std::string cmd);
        ~Command();     
};
