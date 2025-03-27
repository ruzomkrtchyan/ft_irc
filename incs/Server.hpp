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
#include <errno.h>
#include <stdio.h>
#include <sstream>
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
		Manager							*cmdManager;
		
		void		create_sock();
		void		new_client();
		void		receiving_data(int i);
		void		handle_msg(Client &client, std::string msg);
		
		public:
		std::map<std::string, Channel*>	_channels;
		Server(int prt, std::string passw);
		~Server();
		std::string	getPassword();
		void		connect();
		void		removeClient(Client& client);
		Client*		get_client_bynick(std::string &nick);

		Channel*	getChannel(const std::string& name);
		Channel*	createChannel(const std::string& name, Client& creator);
		void		checkForClosedChannels();
		void		leaveAllChannels(Client& client);
};

#endif
