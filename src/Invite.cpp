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
    if (!client.isOperator())
    {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), "INVITE"));
        return;
    }
    
    std::string nick = args[1];
    std::string chn = args[2];

    Client* cl = serv.get_client_bynick(nick);
    Channel* channel = serv.getChannel(chn);
    if (!cl || !channel)
    {
        client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), "INVITE"));
        return;
    }
    if (channel->isMember(*cl))
    {
        client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), "INVITE", nick));
        return;
    }

    cl->sendMessage("INVITE " + client.getNickname() + " " + chn);
}