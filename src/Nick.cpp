#include "Command.hpp"

Nick::Nick()
{}

Nick::~Nick()
{}

bool isValidNickname(const std::string& nickname)
{
	// Nicknames must be 1-9 characters long and contain only allowed characters.
	if (nickname.empty() || nickname.length() > 9)
		return false;

	// First character must be a letter
	if (!std::isalpha(nickname[0]))
		return false;

	// Check the rest of the nickname
	for (std::size_t i = 0; i < nickname.length(); ++i)
	{
		char c = nickname[i];
		if (!std::isalnum(c) && c != '-' && c != '_')
			return false;
	}

	return true;
}

void Nick::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
	(void)serv;

	if (!client.isAuth())
	{
		send(client.getFd(), ":server 451 * :You have not registered\r\n", 40, 0);
		return;
	}
	if (args.size() < 2 || args[1].empty())
	{
		send(client.getFd(), ":server 431 * :No nickname given\r\n", 34, 0);
		return;
	}

	std::string newNick = args[1];

	// Check if nickname is already in use
	if (serv.isClientTaken(newNick))
	{
		std::string msg = ":server 433 * " + newNick + " :Nickname is already in use\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		return;
	}

	// Check for valid nickname characters
	if (!isValidNickname(newNick))
	{
		std::string msg = ":server 432 * " + newNick + " :Erroneous nickname\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		return;
	}

	// Check if user is already registered
	if (client.isFullyRegistered())  
	{
		std::string oldNick = client.getNickname();
		client.setNickname(newNick);
		std::string message = ":" + oldNick + " NICK " + newNick + "\r\n";
	}
	else
	{
		client.setNickname(newNick);
		std::string msg = ":server 001 " + newNick + " :Your nickname has been set. Now use USER <username> 0 * <realname>\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
	}

	std::cout << "Client " << client.getFd() << " set nickname: " << client.getNickname() << std::endl;
}
