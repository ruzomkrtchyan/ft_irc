#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <map>

class Client
{
	public:
		int			sockfd;
		int			authRetries;
		bool		authenticated;
		bool		isRegistered;
		bool		is_operator;
		std::string nickname;
		std::string username;
		std::string realname;
		std::string ip_address;

		Client();
		Client(int fd, struct sockaddr_in client_addr);

		int	getFd() const;
		std::string	getIp() const;
		std::string	getNickname() const;
		std::string getUsername() const;
		std::string getPrefix(const Client &client); //cpp fileum implementel
		std::string resolveHostname(const std::string &ip);
		bool	isAuth() const;
		bool	isFullyRegistered() const;
		bool	isOperator() const;

		void	setNickname(const std::string &nick);
		void	setUsername(const std::string &user);
		void	setRealname(const std::string &name);
		void	setOperator();
		void 	authenticate();
		void	registerClient();
		void	sendMessage(const std::string& message);
		void	leaveAllChannels();
};

#endif