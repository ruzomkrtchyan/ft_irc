#include "Command.hpp"

Join::Join()
{}

Join::~Join()
{}

void Join::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{

    if (!client.isRegistered)
    {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }

    if (args.empty())
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
        return;
    }
    
    std::string channelName = args[1];

    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
    {
        client.sendMessage(ERR_BADCHANMASK(client.getNickname(), channelName));
    }
    Channel* channel = serv.getChannel(channelName);
    if (!channel)
    {
        channel = serv.createChannel(channelName, client);
    }
    if (channel->isClientInChannel(client))
    {
        client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), client.getNickname(), channelName));
    }

    if (channel->isInviteOnly())
    {
        client.sendMessage(ERR_INVITEONLYCHAN(client.getNickname(), channelName));
    }
    if (channel->isFull())
    {
        client.sendMessage(ERR_CHANNELISFULL(client.getNickname(), channelName));
    }
    // if (!pass.empty() && channel->getPassword() != pass)
    // {
    //     client.sendMessage(ERR_BADCHANNELKEY(client.getNickname(), name, "Cannot join channel (+k)"));
    //     continue;
    // }
    channel->addMember(client);
}
