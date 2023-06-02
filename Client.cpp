/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nali <nali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:46:25 by nali              #+#    #+#             */
/*   Updated: 2023/06/02 11:46:34 by nali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{}

Client::Client(int fd)
{
    this->fd		= fd;
    this->auth		= false;
	this->hasInfo	= false;
    this->registrationFlag = false;
    this->_ct       = 0;
    
    struct sockaddr_in my_addr;
    socklen_t addr_len = sizeof(my_addr);
    getsockname(this->fd,(sockaddr *)&my_addr, &addr_len);
    inet_ntop(AF_INET, &my_addr.sin_addr, this->ip_addr, sizeof(ip_addr));
    this->port = ntohs(my_addr.sin_port); 
}

Client::~Client()
{}

Client Client::operator=(Client other)
{
    this->fd = other.fd;
    return (*this);
}

char *Client::get_ip_addr()
{   return (this->ip_addr); }

int Client::get_port()
{   return (this->port); }

int Client::get_socket()
{   return (this->fd); }

int Client::get_auth()
{   return (this->auth); }

std::string& Client::get_MsgFrmClient()
{   return (this->_readmsg);        }

bool& Client::get_passFlag() 
{ return (this->passFlag);  }

int Client::get_count() 
{ return (this->_ct);     }

std::string	Client::get_nickname()
{   return (this->_nickname);   }

std::string	Client::get_old_nickname()
{ return (this->_old_nickname); }

std::string Client::get_username()
{ return (this->_username);     }

std::string Client::get_realname()
{ return (this->_realname);     }

bool& Client::has_all_info()
{   return (this->hasInfo);     }

int Client::get_info()
{   return (this->hasInfo);     }

//////////////////////////Setters////////////////////

void Client::set_auth(int val)
{   this->auth = val;               }

void Client::set_MsgInClient(std::string const &buf)
{   this->_readmsg += buf;          }


void Client::set_nickname(std::string nick)
{	this->_nickname = nick;			}

void Client::set_old_nickname(std::string old_nick)
{	this->_old_nickname = old_nick;	}

void Client::set_username(std::string username)
{	this->_username = username;		}

void Client::set_realname(std::string realname)
{	this->_realname = realname;		}

void	Client::set_passFlag(bool flag)
{
	passFlag = flag;
}

bool& Client::first_invite()
{   return (this->welcomeFlag);   }


void Client::set_count(int n)
{
	_ct += n;
}

bool& Client::isRegistration() 	
{ return (this->registrationFlag); }


int	Client::is_valid() const
{
	if (_username.empty())
		return (FAILURE);
	if (_nickname.empty())
		return (FAILURE);
    if (this->_ct < 3)
		return (FAILURE);
	return (SUCCESS);
}

void Client::SendReply(int recipient_fd, std::string msg)
{
    send(recipient_fd, msg.c_str(), msg.length(), 0);
}
