#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <iostream> 
#include <map>
#include "Server.hpp"
#include "Nick.hpp"
#include "Pass.hpp"

class Command;

class Manager
{
	private:
		std::map<std::string, Command*> commands;
	public:
		Manager();
		~Manager();
		void execute(std::string &command, Client &client, Server &serv, std::vector<std::string> args);
};

#endif