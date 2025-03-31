#include "Command.hpp"

User::User()
{}

User::~User()
{}

void User::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    (void) serv;
    
    if (client.isFullyRegistered())
    {
        client.sendMessage(ERR_ALREADYREGISTERED(client.getNickname()));
        return;
    }
    
    if (!client.isAuth())
    {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }
    
    if (client.getNickname().empty())
    {
        client.sendMessage(ERR_NONICKNAMEGIVEN(client.getNickname()));
        return;
    }
    
    if (args.size() < 5)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
        return;
    }

    client.setUsername(args[1]);
    client.setRealname(args[4]);

    client.registerClient();
    client.sendMessage(RPL_WELCOME(client.getNickname()));
}
