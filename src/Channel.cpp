#include "Channel.hpp"


Channel::Channel(const std::string& name, Client& creator) : _name(name),inviteOnly(false)
{
    addMember(creator);
}

bool Channel::isMember(const Client& client) const
{
    return _members.find(client.getNickname()) != _members.end();
}

void Channel::addMember(Client& client)
{
    _members[client.getNickname()] = &client;
}

void Channel::broadcast(const std::string& message, const Client& sender)
{
    for (std::map<std::string, Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        if (it->second != &sender) 
            it->second->sendMessage(message);
    }
}

std::string Channel::getTopic() const
{
    return _topic;
}

void Channel::set_inviteOnly()
{
    inviteOnly = true;
}

bool Channel::isInviteOnly()
{
    return inviteOnly;
}

void Channel::addInvite(std::string &nick)
{
    invited_users.insert(nick);
}