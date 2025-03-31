#include "Command.hpp"

Pass::Pass()
{}

Pass::~Pass()
{}

void Pass::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    
    if (client.isAuth())
    {
        std::string msg = ":server 462 " + client.getNickname() + " :You may not reregister\r\n";
        send(client.getFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    if (args.size() < 2)
    {
        std::string msg = ":server 461 PASS :Not enough parameters\r\n";
        send(client.getFd(), msg.c_str(), msg.size(), 0);
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
        send(client.getFd(), ":server NOTICE AUTH :Password accepted, please enter your nickname with NICK <nickname>\r\n", 90, 0);
        return;
    }
    else 
    {
        client.increment_retries();
        if (client.getAuthRetries() < 4)
        {
            std::string msg = ":server 464 * :Password incorrect\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
        }    
        else
        {
            std::string msg = ":server 464 * :Password incorrect\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
            
            close(client.getFd()); 
            client.setAuthRetries(0); 
            serv.removeClient(client);
        }
    }
}