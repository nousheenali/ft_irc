/* INVITE command: Invite a user to a channel.
 *
 * Syntax:          INVITE <nickname> <channel>
 *
 */

#include "Server.hpp"
#include "Command.hpp"

std::vector<std::string> convert_to_vector(std::string msg);
std::vector<std::string> split(const std::string &s, char delimiter);

int invite(Server *server, int client_fd, msg_struct cmd_infos)
{
    std::vector<std::string> param_splitted = convert_to_vector(cmd_infos.parameter);
    Client *cl = server->GetClient(client_fd);

    // Validate the command parameters
    if (param_splitted.size() != 2)
    {
        server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
        return (FAILURE);
    }

    // Check if invited user exists
    Client *invitedClient = server->GetClient(param_splitted[0]);
    if (!invitedClient)
    {
        server->SendReply(client_fd, ERR_NOSUCHNICK(cl->get_nickname()));
        return (FAILURE);
    }

    // Check if channel exists
    Channel *channel = server->GetChannel(param_splitted[1]);
    if (channel) 
    {
        std::string channelMode = channel->getChannelMode();
        // Check if user is a member of the channel
        if (!channel->isMember(cl->get_nickname()))
        {
            server->SendReply(client_fd, ERR_NOTONCHANNEL(param_splitted[1]));
            return (FAILURE);
        }
        if (channelMode.find('i') != std::string::npos) // checking for invite only mode
        {
            // The channel is invite-only. Ensure the user is an operator.
            if (!channel->isOperator(cl->get_nickname()))
            {
                server->SendReply(client_fd, ERR_CHANOPRIVSNEEDED(server->GetServerName(), cl->get_nickname()));
                return (FAILURE);
            }
        }

        // if (channelMode.find('l') != std::string::npos) // checking for limited channel mode
        // {
        //     // The channel is limited. Check if the channel is full.
        //     if (channel->isFull())
        //     {
        //         server->SendReply(client_fd, "-!- " + cl->get_nickname() + ": Cannot join channel " + channel->get_channel_name() + " (+l) - channel is full");
        //         return (FAILURE);
        //     }
        // }

        // Check if invited user is already a member of the channel
        if (channel->isMember(invitedClient->get_nickname()))
        {
            
            server->SendReply(client_fd, ERR_USERONCHANNEL(server->GetServerName(), cl->get_nickname(), invitedClient->get_nickname() + " already member of that channel"));
            return (FAILURE);
        }
    }

    //according rfc2812,invite can be sent to a channel that has not been created also
    if (param_splitted[1][0] != '#')
    {
        server->SendReply(client_fd, ERR_NOSUCHCHANNEL(cl->get_nickname(), param_splitted[1]));
        return (FAILURE);
    }
    // Add the channel name to the invited user's invite list
    invitedClient->addInvite(param_splitted[1]);

     // Send the INVITE message to the invited user
    server->SendReply(invitedClient->get_socket(), RPL_INVITING(cl->get_nickname(), invitedClient->get_nickname(), param_splitted[1])); // send invite to user
    
    // Send a confirmation message to the user that sent the INVITE
    server->SendReply(client_fd, RPL_INVITING2(server->GetServerName(), invitedClient->get_nickname(), param_splitted[1]));
    
    return (SUCCESS);
}
