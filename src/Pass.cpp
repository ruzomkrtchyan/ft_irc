#include "Command.hpp"

Pass::Pass()
{}

Pass::~Pass()
{}

void Pass::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{

    if (client.isAuth())
    {
        client.sendMessage(ERR_ALREADYREGISTERED(client.getNickname()));
        return;
    }

    if (args.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
        return;
    }

    std::string pass;
    if (args[1][0] == ':')
        pass = args[1].substr(1);
    else
        pass = args[1];

    if (pass == serv.getPassword())
    {
        client.authenticate();
        client.setAuthRetries(0);
        // client.sendMessage(":server NOTICE AUTH :Password accepted, please enter your nickname with NICK <nickname>\r\n");
        return;
    }
    else 
    {
        client.increment_retries();
        if (client.getAuthRetries() < 4)
        {
            client.sendMessage(ERR_PASSWDMISMATCH(client.getNickname()));
        }    
        else
        {
            client.sendMessage(ERR_PASSWDMISMATCH(client.getNickname()));
            close(client.getFd()); 
            client.setAuthRetries(0); 
            serv.removeClient(client);
        }
    }
}
