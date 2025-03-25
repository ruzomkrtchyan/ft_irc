#include "Command.hpp"

Topic::Topic(){}

Topic::~Topic(){}

void Topic::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
		return;
    }

    std::string chn = args[1];

    Channel* channel = serv.getChannel(chn);
    if (!channel)
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), "TOPIC"));
        return;
    }

    if (!channel->isMember(client))
    {
        client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), chn));
        return;
    }

    if (args.size() == 2)
    {
        if(channel->getTopic().empty())
            client.sendMessage(RPL_NOTOPIC(client.getNickname(), chn));
        else
            client.sendMessage(RPL_TOPIC(client.getNickname(), chn, channel->getTopic()));
        return;
    }

    // if (channel.findmode('t') && !client.isOperator())
    // {
    //     client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), "TOPIC"));
    //     return;
    // }

    std::string topic;
	if (args[2][0] == ':')
	{
		topic = args[2].substr(1);
		for (size_t i = 3; i < args.size(); ++i)
			topic += " " + args[i];
	}
	else
		topic = args[2];
    
    channel->setTopic(topic);
    channel->broadcast(":" + client.getNickname() + " TOPIC " + chn + " :" + topic, client); 
}