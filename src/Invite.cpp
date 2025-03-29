#include "Command.hpp"

Invite::Invite(){}

Invite::~Invite(){}

void Invite::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return; 
    }
    
    std::string nick = args[1];
    std::string chn = args[2];
    
    Client* cl = serv.get_client_bynick(nick);
    if (!cl)
    {
        client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), "INVITE"));
        return;
    }
    
    Channel* channel = serv.getChannel(chn);
    if (!channel)
    {
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), "INVITE"));
        return;
    }
    
    if (!channel->isMember(client))
    {
        client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), chn));
        return;
    }

    if (channel->isMember(*cl))
    {
        client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), "INVITE", nick));
        return;
    }

    if (channel->isInviteOnly() && !channel->isOperator(client))
    {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), "INVITE"));
        return;
    }

    channel->addInvite(nick);

    cl->sendMessage(":" + client.getPrefix(client) + " INVITE " + cl->getNickname() + " :" + chn);

    client.sendMessage(RPL_INVITING(client.getNickname(), nick, chn));
}
