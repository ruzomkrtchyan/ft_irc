#include "Command.hpp"

Nick::Nick()
{}

Nick::~Nick()
{}

void Nick::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
	(void)serv;

	if (!client.isAuth())
	{
		send(client.getFd(), "You must authenticate first with PASS.\n", 39, 0);
		return;
	}
	if (args.size() > 1)
	{
		client.setNickname(args[1]); //should set clients.nickname also
		send(client.getFd(), "Nickname set successfully! Please enter USER <username> 0 * <realname>\n", 54, 0);
		std::cout << "Client " << client.sockfd << " set nickname: " << client.getNickname() << std::endl;
	}
	else
		send(client.getFd(), "Invalid nickname. Please try again.\n", 36, 0);
	// else
	// 	send(client.sockfd, "You must set your nickname using NICK <name>\n", 44, 0);
}