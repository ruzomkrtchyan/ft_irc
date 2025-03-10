#include "Manager.hpp"
#include "Server.hpp"

Manager::Manager()
{
	commands["PASS"] = new Pass();
	commands["NICK"] =  new Nick();
	commands["USER"] =  new User();
}

Manager::~Manager()
{
	for(std::map<std::string, Command*>::iterator it = commands.begin(); it != commands.end(); ++it)
		delete it->second;
}

void Manager::execute(std::string &command, Server &serv, Client &client, std::vector<std::string> args)
{
	if (!client.isFullyRegistered())
	{
		if( command != "PASS" && command != "NICK" && command != "USER")
		{
			send(client.getFd(), "451 :You have not registered. Use PASS, NICK, and USER.\r\n", 60, 0);
            return;
		}
	}
	
	if(commands.find(command) != commands.end())
		commands[command]->execute(serv, client, args);
	else
		send(client.getFd(), "ERROR: Unknown command\r\n", 24, 0);
}