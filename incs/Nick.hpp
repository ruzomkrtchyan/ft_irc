#ifndef NICK_HPP
#define NICK_HPP

#include "Command.hpp"

class Nick : public Command
{
	public:
		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

#endif