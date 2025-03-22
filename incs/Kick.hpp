#ifndef KICK_HPP
#define KICK_HPP

#include "Command.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Kick : public Command {
    public:
        Kick();
        ~Kick();

        void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

#endif
