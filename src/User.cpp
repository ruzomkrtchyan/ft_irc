#include "User.hpp"

void User::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
	(void)serv;
	if (!client.isAuth())
	{
		send(client.getFd(), "You must authenticate first with PASS.\n", 40, 0);
		return;
	}
	if (client.getNickname().empty())
	{
		send(client.getFd(), "You must set a nickname first with NICK.\n", 45, 0);
        return;
	}
	if (args.size() < 5)
	{
		send(client.getFd(), "ERROR: Invalid USER format. Usage: USER <username> 0 * <realname>\n", 64, 0);
		return;
	}

	client.setUsername(args[1]);
	client.setRealname(args[4]);

	client.registerClient();
	send(client.getFd(), "Registration complete! Welcome to the IRC Server.\n", 55, 0);
}