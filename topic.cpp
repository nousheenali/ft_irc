

#include "Server.hpp"
#include "Command.hpp"

std::vector<std::string> convert_to_vector(std::string msg);
std::vector<std::string> split(const std::string &s, char delimiter);

int topic(Server *server, int client_fd, msg_struct cmd_infos)
{
    std::cout << "PARAMS :" << cmd_infos.parameter << std::endl;

    std::vector<std::string> param_splitted = convert_to_vector(cmd_infos.parameter);
    Client *cl = server->GetClient(client_fd);

    std::cout << "size :" << param_splitted.size() << std::endl;

    std::cout
        << "channel name :" << param_splitted[0] << std::endl;
    return 1;

    if (param_splitted.size() < 1 || param_splitted.size() > 2)
    {
        server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
        return (FAILURE);
    }

    std::string channelName = param_splitted[0];
    std::map<std::string, Channel *>::iterator it = server->GetChannelList().find(channelName);

    if (it == server->GetChannelList().end())
    {
        server->SendReply(client_fd, ERR_NOSUCHCHANNEL(cl->get_nickname(), channelName));
        return (FAILURE);
    }

    Channel *ch = it->second;

    if (!ch->isMember(cl->get_nickname()))
    {
        server->SendReply(client_fd, ERR_NOTONCHANNEL(ch->get_channel_name()));
        return (FAILURE);
    }

    // If 2 parameters are given, the client wants to change the topic
    if (param_splitted.size() == 2)
    {
        // Check for the operator privileges
        if (!ch->isOperator(cl->get_nickname()))
        {
            server->SendReply(client_fd, ERR_CHANOPRIVSNEEDED(server->getServerIP(), cl->get_nickname()));
            return (FAILURE);
        }

        // Set the new topic
        ch->set_topic(param_splitted[1]);
        server->SendReply(client_fd, RPL_TOPIC(server->getServerIP(), cl->get_nickname(), ch->get_channel_name(), ch->get_topic()));
    }
    else // The client wants to know the current topic
    {
        if (ch->get_topic().empty())
        {
            server->SendReply(client_fd, "331 :no topic set");
        }
        else
        {
            server->SendReply(client_fd, RPL_TOPIC(server->getServerIP(), cl->get_nickname(), ch->get_channel_name(), ch->get_topic()));
        }
    }

    return (SUCCESS);
}
