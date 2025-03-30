#include "Command.hpp"

Pass::Pass()
{}

Pass::~Pass()
{}

void Pass::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (client.isAuth())
    {
        send(client.getFd(), ":server 451 * :You have not registered\r\n", 38, 0);
        return;
    }
    if (args.size() < 2)
    {
        send(client.getFd(), ":server 461 PASS :Not enough parameters\r\n", 41, 0);
        return;
    }
    std::string pass = args[1];
    if (pass == serv.getPassword())
    {
        client.authenticate();
        send(client.getFd(), ":server NOTICE AUTH :Password accepted, please enter your nickname with NICK <nickname>\r\n", 90, 0);
        return;
    }
    else
	{
        if (client.authRetries < 3)
        {
            std::string msg = ":server 464 " + client.getNickname() + " :Password incorrect\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);

        }    
        serv.removeClient(client);
	}
}