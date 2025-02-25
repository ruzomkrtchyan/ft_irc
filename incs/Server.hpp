#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <cstdlib>
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>
#include <vector>
#include <cstring> 
#include <unistd.h>
#include <map>
#include <sstream>
#include "Client.hpp"

class Client;
class Channel;

class Server
{
	private:
		int		 port;
		int		 sockfd;
		std::string password;
		std::vector<struct pollfd> fds;
		std::map<int, Client> clients;

		void		create_sock();
		void		new_client();
		void		receiving_data(int i);
		std::string trim_p(std::string pass);
		void		client_authentication(int i, std::string msg);
		void		client_nickname(int i, std::string msg);
		void		client_username(int i, std::string msg);
		void		processCommand(Client &client, const std::string &command);

	public:
		Server(int prt, std::string passw);
		void		connect();
};

#endif
