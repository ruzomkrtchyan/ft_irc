#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <iostream> 
#include <map>
#include <vector>
#include "Client.hpp"

class Command;
class Server;

class Manager
{
	private:
		std::map<std::string, Command*> commands;
	public:
		Manager();
		~Manager();
		void execute(std::string &command, Server &serv, Client &client, std::vector<std::string> args);
};

#endif