#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include <set>
#include "Client.hpp"

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::map<std::string, Client*> _members;

        bool                    inviteOnly;
        std::set<std::string>   invited_users;

    public:
        Channel(const std::string& name, Client& creator);
        bool isMember(const Client& client) const;
        void addMember(Client& client);
        void broadcast(const std::string& message, const Client& sender);
        std::string getTopic() const;

        void set_inviteOnly();
        bool isInviteOnly();
        void addInvite(std::string &nick);

};

#endif