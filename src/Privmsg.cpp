#include "Command.hpp"

Privmsg::Privmsg()
{}

Privmsg::~Privmsg()
{}

void Privmsg::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
	if (args.size() < 3)
	{
		client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
		return;
	}

	std::string target = args[1];

	std::string msg;
	if (args[2][0] == ':')
	{
		msg = args[2].substr(1);
		for (size_t i = 3; i < args.size(); ++i)
			msg += " " + args[i];
	}
	else
		msg = args[2];

	if (msg.empty())
	{
		client.sendMessage(ERR_NOTEXTTOSEND(client.getNickname()));
		return;
	}

	if (target[0] == '#')
	{
		if (serv._channels.find(target) == serv._channels.end())
		{
			client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), "PRIVMSG"));
			return;
		}

		Channel *chn = serv._channels[target];
	 
		if (!chn->isMember(client))
		{
			client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), "PRIVMSG"));
			return;
		}

		std::string ch_msg = ":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\n";
		chn->broadcast(ch_msg, client);
	}
	else
	{
		Client* recipient = serv.get_client_bynick(target);
		if (!recipient || !recipient->isFullyRegistered())
		{
			client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), "PRIVMSG"));
			return;
		}
		std::string u_msg = ":" + client.getNickname() + " PRIVMSG " + target + " :" + msg + "\n";
		send(recipient->getFd(), u_msg.c_str(), u_msg.size(), 0);
	}
	
}
// PRIVMSG <target> :<message>
// <target> → A nickname (to message a specific user) or a channel (to message a group).
// <message> → The actual text message.

// PRIVMSG #general :Hello everyone!
// :user1 PRIVMSG #general :Hello everyone!