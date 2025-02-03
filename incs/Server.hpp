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
#include "Client.hpp"

class Client;
class Channel;

class Server
{
    private:
        int         port;
        int         sockfd;
        std::string password;
        std::vector<struct pollfd> fds;
        std::map<int, Client> clients;

        void create_sock();
        void new_client();
        void receiving_data(int i);

    public:
        Server(int prt, std::string passw);
        void connect();

};

#endif