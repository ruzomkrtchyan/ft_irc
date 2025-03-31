#include "Command.hpp"

Ping::Ping()
{}

Ping::~Ping()
{}

void Ping::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    (void) serv;
    if (args.size() < 2) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PING"));
        return;
    }
    std::string msg = args[1];
    std::string res = ":";
    std::string nick = client.getNickname();
    std::string user = client.getUsername();

    if (!nick.empty()) 
        res += nick;
    if (!user.empty())
        res += "!" +  user;

    res += "@" + msg + " PONG :" + msg;

    client.sendMessage(res);
}
