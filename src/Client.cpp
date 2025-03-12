#include "Client.hpp"

Client::Client() : sockfd(-1), authRetries(0), authenticated(false), isRegistered(false), ip_address("0.0.0.0") {}

Client::Client(int fd, struct sockaddr_in client_addr)
	: sockfd(fd), authRetries(0), authenticated(false), nickname(""), username(""), realname("")
{
	// Extract and store IP address from client socket
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
	ip_address = std::string(ip);

	std::cout << "New client connected: " << ip_address << " (FD: " << sockfd << ")" << std::endl;
}

int Client::getFd() const
{ 
	return sockfd; 
}

std::string Client::getIp() const
{
	return ip_address;
}

std::string Client::getNickname() const
{
	return nickname;
}

std::string Client::getUsername() const
{ 
	return username;
}

std::string Client::getPrefix() const
{
    return nickname + "!" + username + "@" + ip_address;
}

bool Client::isAuth() const
{ 
	return authenticated;
}

bool Client::isFullyRegistered() const
{ 
	return isRegistered;
}

void Client::setNickname(const std::string &nick)
{ 
	nickname = nick;
}

void Client::setUsername(const std::string &user)
{ 
	username = user;
}

void Client::setRealname(const std::string &name)
{
	realname = name;
}

void Client::authenticate()
{ 
	authenticated = true; 
}

void Client::registerClient() 
{
	isRegistered = true;
}


void Client::sendMessage(const std::string& message)
{
	send(sockfd, message.c_str(), message.length(), 0);
}