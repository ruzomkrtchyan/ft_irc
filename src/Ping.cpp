#include "Command.hpp"

Ping::Ping()
{}

Ping::~Ping()
{}

void Ping::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    (void) serv;
    std::string msg = args[1];
    std::string res = ":";
    std::string nick = client.getNickname();
    std::string user = client.getUsername();
    if(nick != NULL)
        res += nick;
    if(user != NULL)
        res += "!" +  user;
    res += "@" + msg + " PONG : " + msg;

    client.sendMessage(res);
}