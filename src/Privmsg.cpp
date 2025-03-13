#include "Privmsg.hpp"

Privmsg::Privmsg()
{}

Privmsg::~Privmsg()
{}

void Privmsg::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
        return;
    }

    std::string target = args[1]
    if (target[0] == "#")
    {
        if (serv._channels.find(target) == server._channels.end())
        {
            client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), "PRIVMSG"));
            return;
        }

        Channel chn = serv._channels[target];
     
        if (!chn.isMember(client))
        {
            client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), "PRIVMSG"));
            return;
        }

        std::string msg = client.getNickname() + " PRIVMSG " + target +;
    }
}

PRIVMSG <target> :<message>
<target> → A nickname (to message a specific user) or a channel (to message a group).
<message> → The actual text message.

PRIVMSG #general :Hello everyone!
:user1 PRIVMSG #general :Hello everyone!