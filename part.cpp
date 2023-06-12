/* PART command: user can leave the channel with this command.
 *
 * Syntax:          PART <channel> [<parting message>]
 * Numeric Replies: ERR_NEEDMOREPARAMS (461)
 *
 */

#include "Server.hpp"
#include "Command.hpp"

std::vector<std::string> convert_to_vector(std::string msg);
std::vector<std::string> split(const std::string &s, char delimiter);
std::string ParamsJoin(std::vector<std::string> vec);

int part(Server *server, int client_fd, msg_struct cmd_infos)
{
    Client *cl = server->GetClient(client_fd);

    // Validate the command parameters
    // Split command parameters into channels and optional message
    std::vector<std::string> param_splitted = convert_to_vector(cmd_infos.parameter);

    if (param_splitted.size() < 1 || cmd_infos.parameter.empty())
    {
        server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
        return (FAILURE);
    }
    std::vector<std::string> channelNames = split(param_splitted[0], ',');
    std::string message = (param_splitted.size() > 1) ? ParamsJoin(param_splitted) : "";

    // Iterate over the list of channels
    for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        std::map<std::string, Channel *>::iterator channelIt = server->GetChannelList().find(*it);

        // Check if the channel exists
        if (channelIt == server->GetChannelList().end())
        {
            server->SendReply(client_fd, ERR_NOSUCHCHANNEL(cl->get_nickname(), *it));
            continue;
        }

        // Check if the user is a member of the channel
        if (!channelIt->second->isMember(cl->get_nickname()))
        {
            server->SendReply(client_fd, ERR_NOTONCHANNEL(*it));
            continue;
        }

        // Send part message to all channel members
        std::string partMsg = cl->get_msg_prefix() + " PART " + *it + " " + message + "\r\n";
        for (std::vector<Channel::Channel_Member>::iterator mem_it = channelIt->second->members.begin(); mem_it != channelIt->second->members.end(); ++mem_it)
        {
            server->SendReply(mem_it->user->get_socket(), partMsg);
        }

        // Remove user from the channel
        channelIt->second->removeUser(cl->get_nickname());
        if (channelIt->second->get_total_members() == 0 && channelIt->second->get_channel_name()[0] != '!') //dont destroy safe channels starting with '!'
            server->RemoveChannel(channelIt->second->get_channel_name());
        
    }
    return (SUCCESS);
}
