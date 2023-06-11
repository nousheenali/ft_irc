/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 22:00:24 by nali              #+#    #+#             */
/*   Updated: 2023/06/11 11:00:32 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> convert_to_vector(std::string msg)
{
    std::vector<std::string> vec;
    std::string tmp = "\0";

    for (size_t i = 0; i < msg.length(); i++)
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

    std::cout << "+++++++++++++\n";
    std::cout << "vector size is" << vec.size() << " \n";
    for (it = vec.begin(); it != vec.end(); it++)
        std::cout << "-" << *it << "\n";
    std::cout << "+++++++++++++\n";
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == delimiter)
        {
            tokens.push_back(token);
            token = "";
        }
        else
        {
            token += s[i];
        }
    }
    tokens.push_back(token);
    return tokens;
}

std::string ParamsJoin(std::vector<std::string> vec)
{
    std::vector<std::string>::iterator it;
    std::string text;
    
    for (it = vec.begin() + 1; it != vec.end(); it++)
        text += (*it + " ");
    return text;
}