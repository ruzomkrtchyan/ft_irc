#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

class Client
{
	public:
		int			sockfd;
		bool		authenticated;
		int			authRetries;
		std::string nickname;
		std::string username;
		std::string ip_address;

		Client();
		Client(int fd, struct sockaddr_in client_addr);
};

#endif