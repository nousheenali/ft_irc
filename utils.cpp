/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 22:00:24 by nali              #+#    #+#             */
/*   Updated: 2023/06/15 23:53:25 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <iostream>

// std::vector<std::string> convert_to_vector(std::string msg)
// {
//     std::vector<std::string> vec;
//     std::string tmp = "\0";

//     for (size_t i = 0; i < msg.length(); i++)
//     {
//         if (msg[i] != ' ')
//             tmp.push_back(msg[i]);
//         else
//         {
//             vec.push_back(tmp);
//             tmp.clear();
//         }
//     }
//     if (tmp != "\0")
//         vec.push_back(tmp);
//     return vec;
// }

std::vector<std::string> convert_to_vector(std::string s)
{
    std::vector<std::string> tokens;
    std::string token = "\0";
    for (size_t i = 0; i < s.size(); i++)
    {
        while ((s[i] == ' ' || s[i] == '\t') && i < s.size())
            i++;
        while (s[i] != ' ' && s[i] !='\t' && i < s.size())
        {
            token += s[i];
            i++;
        }
        if ((s[i] == ' ' || s[i] =='\t') && i < s.size())
        {
            tokens.push_back(token);
            token.clear();
        }
    }
    if (token != "\0")
        tokens.push_back(token);
    return tokens;
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


std::vector<std::string> splitlower(const std::string &s, char delimiter) //split and store as lower case
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
            token += tolower(s[i]);
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
    {
        text += (*it + " ");
    }
    return text;
}

std::string& lefttrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
std::string& righttrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}