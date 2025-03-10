#ifndef USER_HPP
#define USER_HPP

#include "Command.hpp"

class User : public Command
{
    public:
        User();
        ~User();
        
        void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

#endif