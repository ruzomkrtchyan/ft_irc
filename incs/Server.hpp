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
#include "Channel.hpp"
// #include "Manager.hpp"

class Manager;

class Server
{
	private:
		int		 						port;
		int								sockfd;
		std::string						password;
		std::vector<struct pollfd>		fds;
		std::map<int, Client>			clients;
		std::map<std::string, Channel*>	_channels;
		Manager							*cmdManager;

		void		create_sock();
		void		new_client();
		void		receiving_data(int i);
		std::string trim_p(std::string pass);
		void		client_authentication(int i, std::string msg);
		void		client_nickname(int i, std::string msg);
		void		client_username(int i, std::string msg);
		void		processCommand(Client &client, const std::string &command);
		void		handle_msg(Client &client, std::string msg);

	public:
		Server(int prt, std::string passw);
		~Server();
		std::string	getPassword();
		void		connect();

		Channel* getChannel(const std::string& name);
		Channel* createChannel(const std::string& name, Client& creator);
};

#endif
