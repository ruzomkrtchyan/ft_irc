#include "Command.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void Kick::execute(Server &serv, Client &client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client.sendMessage("Usage: KICK <channel> <nickname> [reason]");
        return;
    }

    std::string channelName = args[0];
    std::string targetNick = args[1];
    std::string reason = (args.size() > 2) ? args[2] : "No reason";

    Channel* channel = serv.getChannel(channelName);
    if (!channel) {
        client.sendMessage("Error: Channel " + channelName + " does not exist.");
        return;
    }

    if (!channel->isMember(client)) {
        client.sendMessage("Error: You're not in the channel.");
        return;
    }

    if (!channel->isOperator(client)) {
        client.sendMessage("Error: You're not a channel operator.");
        return;
    }

    if (!channel->isMember(*serv.get_client_bynick(targetNick))) {
        client.sendMessage("Error: User " + targetNick + " is not in the channel.");
        return;
    }

    channel->removeMember(client);
    std::string kickMessage = ":" + client.getPrefix(client) + " KICK " + channelName + " " + targetNick + " :" + reason;
    channel->broadcast(kickMessage, client);
    
    // Notify the kicked user
    Client* targetClient = serv.get_client_bynick(targetNick);
    if (targetClient) {
        targetClient->sendMessage(kickMessage);
    }
}
