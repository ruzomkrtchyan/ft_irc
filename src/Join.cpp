#include "Command.hpp"

Join::Join()
{}

Join::~Join()
{}

void Join::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
        return;    
    }

    std::string channelName = args[1];

    Channel* channel = serv.getChannel(channelName);
    if (channel && channel->isMember(client))
        return;

    if (!channel)
    {
        std::cout << "Channel does not exist. Creating a new one..." << std::endl;  // Debugging line
        channel = serv.createChannel(channelName, client);
        client.setOperator();
    }

    if ((channelName[0] != '#' && channelName[0] != '&'))
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }

    // Channel* channel = serv.getChannel(channelName);
    // if (channel && channel->isMember(client))
    //     return;

    // if (!channel)
    // {
    //     std::cout << "Channel does not exist. Creating a new one..." << std::endl;  // Debugging line
    //     channel = serv.createChannel(channelName, client);
    // }

    channel->addMember(client);

    client.sendMessage(":" + client.getPrefix(client) + " JOIN " + channelName);

     if (!channel->getTopic().empty()) {
        client.sendMessage(RPL_TOPIC(client.getNickname(),channelName, channel->getTopic()));
    }

    channel->broadcast(":" + client.getPrefix(client) + " JOIN " + channelName, client);
}
