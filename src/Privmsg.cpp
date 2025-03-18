#include "Privmsg.hpp"

Privmsg::Privmsg()
{}

Privmsg::~Privmsg()
{}

void Privmsg::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (args.size() < 3 || args[2][0] != ":")
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
        return;
    }

    std::string target = args[1];

    std::string msg = args[2].substr(1);
    // if (args[2][])
    //     client.sendMessage(ERR_NOTEXTTOSEND(client.getNickname()));

    for (size_t i = 3; i < args.size(); ++i)
        msg += " " + args[i];

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

        std::string msg = ":" + client.getNickname() + " PRIVMSG " + target + " " + msg + "\n";
        chn.broadcast(msg, chn);
    }
    else
    {
        Client* recipient = serv.get_client_bynick(target);
        if (!recipient)
            client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), "PRIVMSG"));
        std::string msg = ":" + client.getNickname() + " PRIVMSG " + target + " " + msg + "\n";
        send(recipient->getFd(), msg.c_str(), msg.size(), 0);
    }
    
}

// PRIVMSG <target> :<message>
// <target> → A nickname (to message a specific user) or a channel (to message a group).
// <message> → The actual text message.

// PRIVMSG #general :Hello everyone!
// :user1 PRIVMSG #general :Hello everyone!