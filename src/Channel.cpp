#include "Channel.hpp"


Channel::Channel(const std::string& name, Client& creator) : _name(name),inviteOnly(false)
{
    addMember(creator);
    _operators.insert(creator.getNickname());
}

Client* Channel::getClientByNickname(const std::string& nickname) {
    std::map<std::string, Client*>::iterator it = this->_members.find(nickname);
    if (it != this->_members.end())
        return it->second;
    return NULL; 
}

bool Channel::isMember(const Client& client) const
{
    return _members.find(client.getNickname()) != _members.end();
}

bool Channel::isOperator(const Client& client) const {
    return _operators.find(client.getNickname()) != _operators.end();
}

void Channel::addMember(Client& client)
{
    _members[client.getNickname()] = &client;
}

void Channel::removeMember(const Client& client) {
    _members.erase(client.getNickname());
    _operators.erase(client.getNickname());  // Remove from operator list if needed
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

void Channel::setTopicRestricted(bool value)
{
    _topicRestricted = value;
}

void Channel::setTopic(std::string &topic)
{
    _topic = topic;
}

void Channel::set_inviteOnly(bool value)
{
    inviteOnly = value;
}

bool Channel::isInviteOnly()
{
    return inviteOnly;
}

void Channel::addInvite(std::string &nick)
{
    invited_users.insert(nick);
}

void Channel::setUserLimit(int limit)
{
    _userLimit = limit;
}

void Channel::setPassword(const std::string& pass)
{
    _password = pass;
}

void Channel::removePassword()
{
    this->_password.clear();
}

void Channel::setOperator(Client& client, bool isOp)
{
    if (isOp)
        this->_operators.insert(client.getNickname()); // Add operator
    else
        this->_operators.erase(client.getNickname()); // Remove operator
}

bool Channel::isClientInChannel(Client& client) const
{
    return (_members.find(client.getNickname()) != _members.end());
}

bool Channel::isFull() const
{
    return (_members.size() >= USER_LIMIT);
}


std::string Channel::getModes() const {
    std::string modes = "+";
    if (_inviteOnly) modes += "i";
    if (_topicRestricted) modes += "t";
    if (!_password.empty()) modes += "k";
    if (_userLimit > 0) modes += "l";
    return modes;
}
