#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <cstdlib>
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <map>

class Client;
class Channel;

class Server
{
    private:
        int         port;
        int         sockfd;
        std::string password;
        std::vector<struct pollfd> fds;

        int create_sock();

    public:
        Server(int prt, std::string passw);
};

#endif