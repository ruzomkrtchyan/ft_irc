#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "replice.hpp"

class Command
{
    public:
        virtual ~Command();
        virtual void execute(Server &serv, Client &client, const std::vector<std::string>& args) = 0; 
};

#endif