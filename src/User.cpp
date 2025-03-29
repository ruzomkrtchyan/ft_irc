#include "Command.hpp"

User::User()
{}

User::~User()
{}

void User::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
	(void) serv;
	if (client.isFullyRegistered())
	{
		client.sendMessage(ERR_ALREADYREGISTERED(client.getNickname()));
		return;
	}
	if (!client.isAuth())
	{
		send(client.getFd(), ":server 451 * :You have not registered\r\n", 38, 0);
		return;
	}
	if (client.getNickname().empty())
	{
		send(client.getFd(), ":server 431 * :No nickname given\r\n", 33, 0);
        return;
	}
	if (args.size() < 5)
	{
		send(client.getFd(), ":server 461 USER :Not enough parameters\r\n", 41, 0);
		return;
	}

	client.setUsername(args[1]);
	client.setRealname(args[4]);

	client.registerClient();
	client.sendMessage(RPL_WELCOME(client.getNickname()));
}