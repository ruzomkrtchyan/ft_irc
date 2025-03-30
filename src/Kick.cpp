#include "Command.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void Kick::execute(Server &serv, Client &client, const std::vector<std::string>& args) {
    if (args.size() < 3) {
        client.sendMessage("Usage: KICK <channel> <nickname> [reason]");
        return;
    }

    std::string channelName = args[1];
    std::string targetNick = args[2];
    std::string reason = (args.size() > 3) ? args[3] : "No reason";

    Channel* channel = serv.getChannel(channelName);
    if (!channel) {
        client.sendMessage("Error: Channel " + channelName + " does not exist.");
        return;
    }

    if (!channel->isMember(client)) {
        client.sendMessage("Error: You're not in the channel.");
        return;
    }

    Client* targetClient = serv.get_client_bynick(targetNick);
    if (!channel->isOperator(client)) {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
        return;
    }

    if (!channel->isClientInChannel(*targetClient)) 
    {
        client.sendMessage(ERR_USERNOTINCHANNEL(targetNick, channelName, "KICK"));
        return;
    }

    channel->removeMember(*targetClient);
    std::string kickMessage = ":" + client.getPrefix(client) + " KICK " + channelName + " " + targetNick + " :" + reason;
    channel->broadcast(kickMessage, client);
    
    if (targetClient) {
        targetClient->sendMessage(kickMessage);
    }
}
