#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <cstdlib>
#include <fcntl.h>
#include <poll.h>

class Server
{
    public:
        std::string password;
        int port;


};

#endif