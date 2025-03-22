#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
#include <string>
#include "Client.hpp"

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::map<std::string, Client*> _members;
        std::set<std::string> _operators;
    public:
        Channel(const std::string& name, Client& creator);
        bool isMember(const Client& client) const;
        bool isOperator(const Client& client) const;
        void addMember(Client& client);
        void removeMember(const Client& client);
        void broadcast(const std::string& message, const Client& sender);
        std::string getTopic() const;
};

#endif