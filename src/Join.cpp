#include "Join.hpp"

Join::Join()
{}

Join::~Join()
{}

void Join::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (args.empty())
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
        return;    
    }

    std::string channelName = args[0];

    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
        return;
    }

    Channel* channel = serv.getChannel(channelName);
    if (channel && channel->isMember(client))
        return;

    if (!channel)
        channel = serv.createChannel(channelName, client);

    channel->addMember(client);

    client.sendMessage(":" + client.getPrefix() + " JOIN " + channelName);

     if (!channel->getTopic().empty()) {
        client.sendMessage(RPL_TOPIC(channelName, channel->getTopic()));
    }

    channel->broadcast(":" + client.getPrefix() + " JOIN " + channelName, client);
}
