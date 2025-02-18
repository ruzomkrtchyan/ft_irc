#include "Client.hpp"

Client::Client() : sockfd(-1), authenticated(false), authRetries(0), ip_address("0.0.0.0") {}

Client::Client(int fd, struct sockaddr_in client_addr)
	: sockfd(fd), authenticated(false), authRetries(0)
{
	// Extract and store IP address from client socket
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
	ip_address = std::string(ip);

	std::cout << "New client connected: " << ip_address << " (FD: " << sockfd << ")" << std::endl;
}

