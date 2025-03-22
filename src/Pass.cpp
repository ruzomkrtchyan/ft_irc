#include "Command.hpp"

Pass::Pass()
{}

Pass::~Pass()
{}

void Pass::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (client.isAuth())
    {
        send(client.getFd(), "You are already authenticated.\n", 31, 0);
        return;
    }
    if (args.size() < 2)
    {
        send(client.getFd(), "Usage: PASS <password>\n", 23, 0);
        return;
    }
    std::string pass = args[1];
    if (pass == serv.getPassword())
    {
        client.authenticate();
        client.authRetries = 0;
        send(client.getFd(), "Password accepted. Please enter NICK <yournickname> \n", 53, 0);
        return;
    }
    else
	{
		if (client.authRetries < 2)
		{
            client.authRetries++;
			send(client.getFd(), "Incorrect password, please enter PASS <password> \n", 50, 0);
		}
		else
		{
			send(client.getFd(), "Incorrect password. Connection closing.\n", 40, 0);
            client.authRetries = 0;
            serv.removeClient(client);
		}
	}
}