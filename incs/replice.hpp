#pragma once
#include <string>

// Numeric replies
#define RPL_WELCOME(source)                     "001 " + source + " :Welcome to the ft_irc Network"
#define RPL_CHANNELMODEIS(channelName, mode_params) "324 " + channelName + " " + mode_params
#define RPL_NOTOPIC(nick, channel)              "331 " + nick + " " + channel + " :No topic is set"
#define RPL_TOPIC(nick, channel, topic)         "332 " + nick + " " + channel + " :" + topic
#define RPL_INVITING(name, nick, ch)            "341 " + name + " " + nick + " " + ch
#define RPL_NAMREPLY(nick, ch, nickList)        "353 " + nick + " = " + ch + " :" + nickList
#define RPL_ENDOFNAMES(name, ch)                "366 " + name +  " " + ch + " :End of /NAMES list"

// Error replies 
#define ERR_NOSUCHNICK(source, command)         "401 " + source + " " + command + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL(source, command)      "403 " + source + " " + command + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(source, command)   "404 " + source + " " + command + " :Cannot send to channel"
#define ERR_NORECIPIENT(source, command)        "411 " + source + " :No recipient given (" + command + ")"
#define ERR_NOTEXTTOSEND(source)                "412 " + source + " :No text to send"
#define ERR_UNKNOWNCOMMAND(source, command)     "421 " + source + " " + command + " :Unknown command"
#define ERR_NONICKNAMEGIVEN(source)             "431 " + source + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(source, command)   "432 " + source + " " + command + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(source, command)      "433 " + source + " " + command + " :Nickname is already in use"
#define ERR_USERNOTINCHANNEL(src, com1, com2)   "441 " + src + " " + com1 + " " + com2 + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(source, command)       "442 " + source + " " + command + " :You're not on that channel"
#define ERR_USERONCHANNEL(src, com1, com2)      "443 " + src + " " + com1 + " " + com2 + " :is already on channel"
#define ERR_NOTREGISTERED(source)               "451 " + source + " :You have not registered"
#define ERR_NEEDMOREPARAMS(source, command)     "461 " + source + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(source)           "462 " + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)              "464 " + source + " :Password incorrect" // 464
#define ERR_KEYSET(source, command)             "467 " + source + " " + command + " :Channel key already set"
#define ERR_CHANNELISFULL(source, command)      "471 " + source + " " + command + " :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE(source, command, msg)   "472 " + source + " " + command + " " + msg
#define ERR_INVITEONLYCHAN(source, command)     "473 " + source + " " + command + " :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY(source, command, msg) "475 " + source + " " + command + " " + msg
#define ERR_NOCHANMODES(channelName)            "477 " + channelName + " :TOPIC mode is OFF ('MODE <channel> +t' is needed)"
#define ERR_BADCHANMASK(source, command)        "476 " + source + " " + command + " :Bad Channel Mask"
#define ERR_CHANOPRIVSNEEDED(source, command)   "482 " + source + " " + command + " :You're not channel operator"
#define ERR_NOTREGISTERED(source)               "451 " + source + " :You have not registered"

#define RPL_MSG(prefix, command, target, message)   ":" + prefix + " " + command + " " + target + " :" + message
#define RPL_KICK(prefix, channel, target, reason)   ":" + prefix + " KICK " + channel + " " + target + " :" + reason
#define RPL_QUIT(prefix, message)                   ":" + prefix + " QUIT :" + message
#define RPL_JOIN(prefix, target)                    ":" + prefix + " JOIN " + target
#define RPL_MODE(prefix, channel, modes)           ":" + prefix + " MODE " + channel + " " + modes
#define RPL_INVITE(prefix, target, channel)         ":" + prefix + " INVITE " + target + " " + channel

