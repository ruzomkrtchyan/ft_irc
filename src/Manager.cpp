#include "Manager.hpp"
#include "Server.hpp"
#include "Command.hpp"

Manager::Manager()
{
	commands["PASS"] = new Pass();
	commands["NICK"] = new Nick();
	commands["USER"] = new User();
	commands["JOIN"] = new Join();
	commands["PRIVMSG"] = new Privmsg();
	commands["KICK"] = new Kick();
	commands["INVITE"] = new Invite();
	commands["TOPIC"] = new Topic();
	commands["QUIT"] = new Quit();
	commands["MODE"] = new Mode();
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
			send(client.getFd(), "451 :You have not registered. Use PASS, NICK, and USER.\r\n", 57, 0);
            return;
		}
	}
	
	if(commands.find(command) != commands.end())
		commands[command]->execute(serv, client, args);
	else
		send(client.getFd(), "ERROR: Unknown command\r\n", 24, 0);
}