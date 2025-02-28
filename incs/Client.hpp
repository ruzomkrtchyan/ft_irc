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
		int			authRetries;
		bool		authenticated;
		bool		isRegistered;
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
		bool	isAuth() const;
		bool	isFullyRegistered() const;

		void	setNickname(const std::string &nick);
		void	setUsername(const std::string &user);
		void 	authenticate();
		void	registerClient();
};

#endif