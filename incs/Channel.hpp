#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
#include <string>
#include <set>
#include "Client.hpp"

#define USER_LIMIT 100 
class Channel
{
    private:
        std::string _name;
        std::string _topic;
        
        bool                    inviteOnly;
        
        std::set<std::string>   invited_users;
        std::set<std::string> _operators;
        bool _inviteOnly;
        bool _topicRestricted;
        std::string _password;
        size_t _userLimit;
        std::string _modes;

    public:
        std::map<std::string, Client*> _members;
        Channel(const std::string& name, Client& creator);
        Client* getClientByNickname(const std::string& nickname);
        bool isMember(const Client& client) const;
        bool isOperator(const Client& client) const;
        bool isClientInChannel(Client& client) const;
        bool isFull() const;
        void addMember(Client& client);
        void removeMember(const Client& client);
        void broadcast(const std::string& message, const Client& sender);
        std::string getTopic() const;
        void setTopic(std::string &topic);

        bool isInviteOnly();
        bool isInvited(std::string nick);

        void set_inviteOnly(bool value);
        void addInvite(std::string &nick);
        void setTopicRestricted(bool value);
        void setPassword(const std::string& pass);
        std::string getPassword() const;
        void removePassword();
        void setUserLimit(int limit);
        void setOperator(Client& client, bool isOp);

        std::string getModes() const;
        bool hasMode(char mode) const;
        void addModes(const std::string& modes);
        void removeModes(const std::string& modes);

};

#endif