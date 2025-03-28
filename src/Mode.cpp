#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"

Mode::Mode() {}

Mode::~Mode() {}

void Mode::execute(Server &serv, Client &client, const std::vector<std::string>& args) {
    if (!client.isFullyRegistered()) {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }

    if (args.size() < 2) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
        return;
    }

    std::string target = args[1];
    std::string mode = args[2];

    // Check if the target is a channel
    if (target[0] == '#') {
        Channel* channel = serv.getChannel(target);
        if (!channel) {
            client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), target));
            return;
        }

        if (!channel->isMember(client)) {
            client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), target));
            return;
        }

        if (!channel->isOperator(client)) {
            client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), target));
            return;
        }

        // Handling channel modes
        if (mode == "+i" || mode == "-i") {
            channel->set_inviteOnly(mode == "+i");
            channel->broadcast(RPL_MODE(client.getPrefix(client), target, mode), client);
        } 
        else if (mode == "+t" || mode == "-t") {
            channel->setTopicRestricted(mode == "+t");
            channel->broadcast(RPL_MODE(client.getPrefix(client), target, mode), client);
        } 
        else if (mode == "+k" || mode == "-k") {
            if (args.size() < 3 && mode == "+k") {
                client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                return;
            }
            if (mode == "+k") {
                channel->setPassword(args[2]);
            } else {
                channel->setPassword("");
            }
            channel->broadcast(RPL_MODE(client.getPrefix(client), target, mode + (mode == "+k" ? " " + args[2] : "")), client);
        } 
        else if (mode == "+o" || mode == "-o") {
            if (args.size() < 3) {
                client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                return;
            }
            Client* targetClient = channel->getClientByNickname(args[2]);
            if (!targetClient) {
                client.sendMessage(ERR_USERNOTINCHANNEL(client.getNickname(), args[2], target));
                return;
            }
            channel->setOperator(*targetClient, mode == "+o");
            channel->broadcast(RPL_MODE(client.getPrefix(client), target, mode + " " + args[2]), client);
        } 
        else if (mode == "+l" || mode == "-l") {
            if (mode == "+l") {
                if (args.size() < 3) {
                    client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                    return;
                }
                int limit = std::atoi(args[2].c_str());
                if (limit < 1) {
                    client.sendMessage(ERR_UNKNOWNMODE(client.getNickname(), mode, "Limit must be greater than 0"));
                    return;
                }
                channel->setUserLimit(limit);
            } else {
                channel->setUserLimit(0); // Remove limit
            }
            channel->broadcast(RPL_MODE(client.getPrefix(client), target, mode + (mode == "+l" ? " " + args[2] : "")), client);
        } 
        else {
            client.sendMessage(ERR_UNKNOWNMODE(client.getNickname(), mode, "Unknown mode"));
        }
    } else {
        client.sendMessage(ERR_UNKNOWNMODE(client.getNickname(), mode, "Target must be a channel"));
    }
}