#include "Client.hpp"
#include "Channel.hpp"

Client::Client() : sockfd(-1), authenticated(false), isRegistered(false), is_operator(false), ip_address("0.0.0.0"), authRetries(0) {}

Client::Client(int fd, struct sockaddr_in client_addr)
	: sockfd(fd), authenticated(false), isRegistered(false), nickname(""), username(""), realname(""), authRetries(0)
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

std::string Client::resolveHostname(const std::string &ip) const
{
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip.c_str());

    struct hostent *host = gethostbyaddr((const void *)&sa.sin_addr, sizeof(sa.sin_addr), AF_INET);
    if (host && host->h_name) {
        return std::string(host->h_name);
    }
    return ip;  // Return IP if hostname resolution fails
}

std::string Client::getPrefix(const Client &client) const
{
	std::string hostname = resolveHostname(client.getIp());
    return nickname + "!" + username + "@" + hostname;
}

bool Client::isAuth() const
{ 
	return authenticated;
}

bool Client::isFullyRegistered() const
{ 
	return isRegistered;
}

// bool	Client::isOperator() const
// {
// 	return is_operator;
// }


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

// void Client::setOperator()
// {
// 	is_operator = true;
// }

void	Client::setAuthRetries(int tmp)
{
	authRetries = tmp;
}

int		Client::getAuthRetries()
{
	return (authRetries);
}

void	Client::increment_retries()
{
	authRetries +=1;
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
	std::string fullMessage = message + "\r\n";
	send(sockfd, fullMessage.c_str(), fullMessage.length(), 0);
}
