/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 22:00:24 by nali              #+#    #+#             */
/*   Updated: 2023/06/02 22:19:43 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <iostream>

std::vector<std::string>  convert_to_vector(std::string msg)
{
    std::vector<std::string> vec;
    std::string tmp = "\0";
    
    for(int i = 0; i < msg.length(); i++)
    {
        if (msg[i] != ' ')
            tmp.push_back(msg[i]);
        else
        {
            vec.push_back(tmp);
            tmp.clear();
        }
    }
    if (tmp != "\0")
        vec.push_back(tmp);
    return vec;
}

void print_vector(std::vector<std::string> vec)
{
    std::vector<std::string>::iterator it;
    
    std::cout <<"+++++++++++++\n";
    std::cout << "vector size is" << vec.size() <<" \n";
    for(it= vec.begin(); it != vec.end(); it++)
        std::cout << "-" << *it <<"\n";
    std::cout <<"+++++++++++++\n";
}