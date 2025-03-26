#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
#include <string>
#include <set>
#include "Client.hpp"

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        
        bool                    inviteOnly;
        std::set<std::string>   invited_users;
        
        std::set<std::string> _operators;
    public:
        std::map<std::string, Client*> _members;
        Channel(const std::string& name, Client& creator);
        bool isMember(const Client& client) const;
        bool isOperator(const Client& client) const;
        void addMember(Client& client);
        void removeMember(const Client& client);
        void broadcast(const std::string& message, const Client& sender);
        std::string getTopic() const;
        void setTopic(std::string &topic);

        void set_inviteOnly();
        bool isInviteOnly();
        void addInvite(std::string &nick);

};

#endif