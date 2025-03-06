#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "Command.hpp"

class Privmsg : public Command
{
    public:
		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

#endif