#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream> 

class Client
{
    public:
        int         sockfd;
        std::string username;
        std::string nickname;
        bool        authenticated;

        Client(int fd);
};


#endif