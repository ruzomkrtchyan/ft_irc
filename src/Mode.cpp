#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"

Mode::Mode() {}

Mode::~Mode() {}

void Mode::execute(Server &serv, Client &client, const std::vector<std::string>& args) 
{
    
    
    if (!client.isFullyRegistered()) {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }
    
    if (args.size() < 2) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
        return;
    }
    
    std::string target = args[1];
    if (target[0] != '#') {
        client.sendMessage(ERR_UNKNOWNMODE(client.getNickname(), "", "Target must be a channel"));
        return;
    }
    
    Channel* channel = serv.getChannel(target);
    if (!channel) {
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), target));
        return;
    }
    if (!channel->isOperator(client)) {
        client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), "MODE"));
        return;
    }    
    if (args.size() < 3) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
        return;
    }

    std::string modeString = args[2];
    bool adding = true;

    for (size_t i = 0; i < modeString.size(); ++i) {
        char modeChar = modeString[i];

        if (modeChar == '+') {
            adding = true;
            continue;
        } 
        else if (modeChar == '-') {
            adding = false;
            continue;
        }

        switch (modeChar) {
            case 'i':
                if (adding) {
                    channel->set_inviteOnly(true);
                    channel->addModes("i");
                } else {
                    channel->set_inviteOnly(false);
                    channel->removeModes("i");
                }
                break;

            case 't':
                if (adding) {
                    channel->setTopicRestricted(true);
                    channel->addModes("t");
                } else {
                    channel->setTopicRestricted(false);
                    channel->removeModes("t");
                }
                break;

            case 'k':
                if (adding) {
                    if (args.size() < 4) {
                        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                        return;
                    }
                    channel->setPassword(args[3]);
                    channel->addModes("k");
                } else {
                    channel->setPassword("");
                    channel->removeModes("k");
                }
                break;

            case 'o': {
                if (args.size() < 4) {
                    client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                    return;
                }
                Client* targetClient = channel->getClientByNickname(args[3]);
                if (!targetClient) {
                    client.sendMessage(ERR_USERNOTINCHANNEL(client.getNickname(), args[3], target));
                    return;
                }

                if (adding) {
                    channel->setOperator(*targetClient, true);
                    channel->addModes("o");
                } else {
                    channel->setOperator(*targetClient, false);
                    channel->removeModes("o");
                }
                break;
            }

            case 'l': {
                if (adding) {
                    if (args.size() < 4) {
                        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
                        return;
                    }
                    int limit = std::atoi(args[3].c_str());
                    if (limit <= 0) {
                        client.sendMessage(ERR_UNKNOWNMODE(client.getNickname(), "l", "Limit must be greater than 0"));
                        return;
                    }
                    channel->setUserLimit(limit);
                    channel->addModes("l");
                } else {
                    channel->setUserLimit(0);
                    channel->removeModes("l");
                }
                break;
            }

            default:
                client.sendMessage(ERR_UNKNOWNMODE(client.getNickname(), std::string(1, modeChar), "Unknown mode character"));
                return;
        }
        std::string modeChange = (adding ? "+" : "-") + std::string(1, modeChar);
        channel->broadcast(RPL_MODE(client.getPrefix(client), target, modeChange), client);
    }
}
