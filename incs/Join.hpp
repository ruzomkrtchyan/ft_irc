#ifndef JOIN_HPP
#define JOIN_HPP

#include "Command.hpp"
#include "Channel.hpp"

class Join : public Command
{
    public:
        Join();
        ~Join();

        void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

#endif