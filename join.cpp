#include "Server.hpp"
#include "Command.hpp"

std::vector<std::string> convert_to_vector(std::string msg);
std::vector<std::string> split(const std::string &s, char delimiter);

void printChannelAndMembers(Server *server)
{
    std::cout << "All current channels in the server : \n";
    std::cout << "--------------------------------------\n";
    for (std::map<std::string, Channel *>::iterator it = server->GetChannelList().begin();
         it != server->GetChannelList().end(); it++)
    {   
        std::cout << "Channel Name: " << it->first << std::endl;
        std::cout << "Channel Members: \n";
        for (std::vector<Channel::Channel_Member>::iterator mem_it = it->second->members.begin();
             mem_it != it->second->members.end(); ++mem_it)
        {
            std::cout << mem_it->user->get_nickname() << ", ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> getChannelKeys(const std::vector<std::string> &param_splitted)
{
    return param_splitted.size() == 2 ? split(param_splitted[1], ',') : std::vector<std::string>();
}

int validateChannelNames(const std::vector<std::string> &channelNames)
{
    for (size_t i = 0; i < channelNames.size(); i++)
    {
        if (channelNames[i].size() < 2 || (channelNames[i][0] != '#' && channelNames[i][0] != '&'))
        {
            return i;
        }
    }
    return -1;
}

void createChannel(Server *server, int client_fd, const std::string &channelName, const std::string &channelKey)
{
    Channel *newCh = new Channel(channelName);
    if (!channelKey.empty())
    {
        newCh->set_key(channelKey);
        newCh->set_key_flag(true);
    }
    Client *cl = server->GetClient(client_fd);
    newCh->addUser(cl);
    server->GetChannelList()[channelName] = newCh;
}

std::string Channel::get_channel_name()
{
    return (name);
}
void Channel::addUser(Client *client)
{
    Channel_Member new_member;
    new_member.user = client;
    this->members.push_back(new_member);
    this->total_members++;
}

void joinChannel(Server *server, int client_fd, Channel *ch, const std::string &channelKey)
{
    if (ch->get_key_flag() && ch->get_key() != channelKey)
    {
        server->SendReply(client_fd, "Error: Incorrect key for channel.");
        return;
    }
    Client *cl = server->GetClient(client_fd);
    if (ch->isMember(cl->get_nickname()))
    {
        cl->SendReply(client_fd, "Error: You are already a member of this channel.");
        return;
    }
    if (ch->get_limit_flag() && ch->get_total_members() >= ch->get_limit())
    {
        server->SendReply(client_fd, "Error: Channel is full.");
        return;
    }
    ch->addUser(cl);
    for (std::vector<Channel::Channel_Member>::iterator mem_it = ch->members.begin();
         mem_it != ch->members.end(); ++mem_it)
    {
        mem_it->user->SendReply(mem_it->user->get_socket(), ":" + mem_it->user->get_nickname() + " JOIN " + ch->get_channel_name());
    }
}

int join(Server *server, int client_fd, msg_struct cmd_infos)
{
    std::vector<std::string> param_splitted = split(cmd_infos.parameter, ' ');

    if (param_splitted.size() > 2 || cmd_infos.parameter.empty())
    {
        server->SendReply(client_fd, ERR_NEEDMOREPARAMS(cmd_infos.cmd));
        return (FAILURE);
    }

    std::vector<std::string> channelNames = split(param_splitted[0], ',');
    std::vector<std::string> channelKeys = getChannelKeys(param_splitted);

    int invalidChannelIndex = validateChannelNames(channelNames);
    if (invalidChannelIndex != -1) 
    {
        server->SendReply(client_fd, ": server_name  403 _nali  " + channelNames[invalidChannelIndex] + " :Bad channel name\n");
        return (FAILURE);
    }

    for (size_t i = 0; i < channelNames.size(); i++)
    {
        std::string channelKey = i < channelKeys.size() ? channelKeys[i] : "";
        std::map<std::string, Channel *>::iterator it = server->GetChannelList().find(channelNames[i]);

        if (it == server->GetChannelList().end())
        {
            std::cout << "creating channel\n";
            createChannel(server, client_fd, channelNames[i], channelKey);
        }
        else
        {
            joinChannel(server, client_fd, it->second, channelKey);
        }
    }
    printChannelAndMembers(server);
    return (SUCCESS);
}