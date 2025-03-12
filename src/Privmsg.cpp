#include "Privmsg.hpp"

Privmsg::Privmsg()
{}

Privmsg::~Privmsg()
{}

void Privmsg::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(" ", "PRIVMSG"));
    }
}

PRIVMSG <target> :<message>
<target> → A nickname (to message a specific user) or a channel (to message a group).
<message> → The actual text message.
