#pragma once
#include <string>

// Numeric replies
#define RPL_WELCOME(source)                     "001 " + source + " :Welcome to the ft_irc Network\r\n"
#define RPL_ENDOFWHO(name, source)              "315 " + name + " " + source + " :End of WHO list\r\n"
#define RPL_CHANNELMODEIS(channelName1, channelName2, mode_params) "324 " + channelName1 + " " + channelName2 + " " + " " + mode_params + "\r\n"
#define RPL_NOTOPIC(nick, channel) (":" + nick + " 331 " + nick + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(nick, channel, topic) (":" + nick + " 332 " + nick + " " + channel + " :" + topic + "\r\n")
#define RPL_INVITING(name, nick, ch)            "341 " + name + " " + nick + " " + ch + "\r\n"
#define RPL_WHOREPLY(name, ch, us, hst, nick, fl, re)"352 " + name + " " + ch + " " + us + " " + hst + " ft_irc " + nick + " " + fl + " :" + re + "\r\n"
#define RPL_NAMREPLY(nick, ch, nickList)        "353 " + nick + " = " + ch + " :" + nickList + "\r\n"
#define RPL_ENDOFNAMES(name, ch)                "366 " + name +  " " + ch + " :End of /NAMES list\r\n"

// Error replies 
#define ERR_NOSUCHNICK(source, command)         "401 " + source + " " + command + " :No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL(source, command)      "403 " + source + " " + command + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(source, command)   "404 " + source + " " + command + " :Cannot send to channel\r\n"
#define ERR_NORECIPIENT(source, command)        "411 " + source + " :No recipient given (" + command + ")\r\n"
#define ERR_NOTEXTTOSEND(source)                "412 " + source + " :No text to send\r\n"
#define ERR_UNKNOWNCOMMAND(source, command)     "421 " + source + " " + command + " :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN(source)             "431 " + source + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(source, command)   "432 " + source + " " + command + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(source, command)      "433 " + source + " " + command + " :Nickname is already in use\r\n"
#define ERR_USERNOTINCHANNEL(src, com1, com2)   "441 " + src + " " + com1 + " " + com2 + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(source, command)       "442 " + source + " " + command + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(src, com1, com2)      "443 " + src + " " + com1 + " " + com2 + " :is already on channel\r\n"
#define ERR_NOTREGISTERED(source)               "451 " + source + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(source, command)     "461 " + source + " " + command + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(source)           "462 " + source + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(source)               source + " :Password incorrect\r\n" //464
#define ERR_KEYSET(source, command)             "467 " + source + " " + command + " :Channel key already set\r\n"
#define ERR_CHANNELISFULL(source, command)      "471 " + source + " " + command + " :Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(source, command, msg)   "472 " + source + " " + command + " " + msg +"\r\n"
#define ERR_INVITEONLYCHAN(source, command)     "473 " + source + " " + command + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(source, command, msg) "475 " + source + " " + command + " " + msg + "\r\n"
#define ERR_NOCHANMODES(channelName)            "477 # " + channelName + " :TOPIC mode is OFF ('MODE <channel> +t' is needed)\r\n"
#define ERR_BADCHANMASK(source, command)        "476 " + source + " " + command + " :Bad Channel Mask\r\n"
#define ERR_CHANOPRIVSNEEDED(source, command)   "482 " + source + " " + command + " :You're not channel operator\r\n"

#define RPL_MSG(prefix, command, target, massage)   ":" + prefix + " " + command + " " + target + " :" + massage + "\r\n"
#define RPL_KICK(prefix, channel, target, reason)   ":" + prefix + " KICK " + channel + " " + target + " :" + reason + "\r\n"
#define RPL_PING(prefix, massage)                   ":" + prefix + " PONG :" + massage + "\r\n"
#define RPL_QUIT(prefix, massage)                   ":" + prefix + " QUIT :Quit: " + massage +"\r\n"
#define RPL_JOIN(prefix, target)                    ":" + prefix + " JOIN " + target + "\r\n"
#define RPL_PART(prefix, target)                    ":" + prefix + " PART " + target + "\r\n"
#define RPL_MODE(prefix, channel, target)           ":" + prefix + " MODE " + channel + " " + target + "\r\n"
#define RPL_INVITE(prefix, target, channel)         ":" + prefix + " INVITE " + target + " " + channel + "\r\n"