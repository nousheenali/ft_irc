/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 09:27:06 by nali              #+#    #+#             */
/*   Updated: 2023/05/25 23:06:31 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

int main(int argc, char **argv)
{
    int port;
    
    if (argc != 3)
    {
        std::cout << "Usage: ./irc PORT PASSWORD." << std::endl; 
        return (1);
    }

    port = atoi(argv[1]);
    if (port < 1024 || port > 65535)
    {
        std::cout << "Port number has to be a value from 1024 to 65535." << std::endl; 
        exit(1);
    }

    // if (strlen(argv[2]) < 10 or strlen(argv[2]) > 15)
    // {
    //     std::cout << "Server password must contain 10-15 characters." << std::endl; 
    //     exit(1);
    // }
 
    try
    {
        Server irc(port, argv[2]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return (0);
}