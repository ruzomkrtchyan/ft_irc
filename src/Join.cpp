#include "Command.hpp"

Join::Join()
{}

Join::~Join()
{}

void Join::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    std::string pass = "";
    if (args.size() == 3)
        pass = args[2];

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
        return;
    }
    Channel* channel = serv.getChannel(channelName);
    if (!channel)
    {
        channel = serv.createChannel(channelName, client);
        channel->setOperator(client, true);
        if (!pass.empty())
        {
            channel->setPassword(pass);
            channel->addModes("k");
        }
    }
    if (channel->hasMode('k'))
        client.sendMessage("has mode k");
    if (channel->isClientInChannel(client) && !channel->isOperator(client))
    {
        client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), client.getNickname(), channelName));
        return ;
    }

    if (channel->isInviteOnly())
    {
        client.sendMessage(ERR_INVITEONLYCHAN(client.getNickname(), channelName));
        return ;
    }
    if (channel->isFull())
    {
        client.sendMessage(ERR_CHANNELISFULL(client.getNickname(), channelName));
        return ;
    }

    if (channel->hasMode('k'))
    {
        if (pass.empty() || channel->getPassword() != pass)
        {
            client.sendMessage(ERR_BADCHANNELKEY(client.getNickname(), channelName, "Cannot join channel (+k)"));
            return;
        }
    }
    
    channel->addMember(client);

    if (!channel->getTopic().empty())
        client.sendMessage(RPL_TOPIC(client.getNickname(), chn, channel->getTopic()));
        
    channel->broadcast(":" + client.getPrefix(client) + " JOIN " + channelName, client);
}
