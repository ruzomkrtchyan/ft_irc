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

    if (args[0] == "0")
    {
        client.leaveAllChannels();
        serv.checkForClosedChannels();
        return;
    }

    std::vector<std::string> channelNames;
    std::vector<std::string> channelKeys;

    size_t pos = args[0].find(',');
    if (pos != std::string::npos)
    {
        channelNames = split(args[0], ',');  // Assuming a split function exists
        if (args.size() > 1)
            channelKeys = split(args[1], ',');
    }
    else
    {
        channelNames.push_back(args[0]);
        if (args.size() > 1)
            channelKeys.push_back(args[1]);
    }

    for (size_t i = 0; i < channelNames.size(); i++)
    {
        std::string name = channelNames[i];
        std::string pass = (i < channelKeys.size()) ? channelKeys[i] : "";

        if (name.empty() || (name[0] != '#' && name[0] != '&'))
        {
            client.sendMessage(ERR_BADCHANMASK(client.getNickname(), name));
            continue;
        }

        Channel* channel = serv.getChannel(name);
        if (!channel)
        {
            channel = serv.createChannel(name, client);
            continue;
        }

        if (channel->isClientInChannel(client))
        {
            client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), client.getNickname(), name));
            continue;
        }

        if (channel->isInviteOnly())
        {
            client.sendMessage(ERR_INVITEONLYCHAN(client.getNickname(), name));
            continue;
        }

        if (channel->isFull())
        {
            client.sendMessage(ERR_CHANNELISFULL(client.getNickname(), name));
            continue;
        }

        // if (!pass.empty() && channel->getPassword() != pass)
        // {
        //     client.sendMessage(ERR_BADCHANNELKEY(client.getNickname(), name, "Cannot join channel (+k)"));
        //     continue;
        // }

        channel->addMember(client);
    }
}
