/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@42abudhabi.ae>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:26:20 by nali              #+#    #+#             */
/*   Updated: 2023/06/04 00:50:41 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <iostream>

class Channel
{
//     private:
//         std::string name;
//         std::string topic;
//         std::string key; //password 
//         int limit;
//         std::vector<int> operators; //to store client_fds of operators
        
//         //modes
//         bool iflag; //invite_only flag --> +i = 1, -i = 0
//         bool tflag; //topic flag
//         bool kflag; // key or password flag
//         bool lflag; //flag to set limit
    
//     public:
//         Channel();
//         Channel(std::string name);
//         //getters and setters
//         bool get_invite_flag();
//         void set_invite_flag(bool i);
//         bool get_topic_flag();
//         void set_topic_flag(bool i);
//         bool get_key_flag();
//         void set_key_flag(bool i);
//         bool get_limit_flag();
//         void set_limit_flag(bool i);

//         std::string get_key();
//         void set_key(std::string key);
//         int get_limit();
//         void set_limit(int val);
        
};

#endif

/* modes explained

    invite  - set to 0 initially, which means anyone can join (-i)
            - can be changed to 1 to make channel invite only (+i)

    topic   - set to 1 initially, which means only operators can change topic (+t)
            - can be changed to 0 to let anybody change the topic (-t)

    key     - set to 0 initially, which means no pwd needed to join channel (-k)
            - can be changed to 1 to set password (+k)

    limit   - set to 0 initially, which means no limit on no.of users(-l)
            - can be changed to 1 to set user limit (+l)
*/

// https://github.com/irssi/irssi/blob/master/docs/manual.txt