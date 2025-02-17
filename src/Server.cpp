#include "Server.hpp"

Server::Server(int prt, std::string passw):port(prt), password(passw)
{
	Server::create_sock();
}

void Server::create_sock()
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // creates an endpoint for communication
	if (sockfd == -1)
	{
		std::cerr << "Failed to create a socket!" << std::endl;
		exit(1); 
	}

	// Need to to set the socket to non-blocking
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	
	struct sockaddr_in serv_addr;
	std::memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY; //tells the server to listen on all available network interfaces (all IP addresses of the machine).

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		std::cerr << "Failed to bind a socket!" << std::endl;
		close(sockfd);
		exit(1);
	}
	if (listen(sockfd, SOMAXCONN) == -1)
	{
		std::cerr << "Failed to listen!" << std::endl;
		close(sockfd);
		exit(1);
	}
}

void Server::connect()
{

	struct pollfd serv_fd; 
	serv_fd.fd = sockfd;
	serv_fd.events = POLLIN; //for reading from the server socket
	fds.push_back(serv_fd);

	while(true)
	{
		int	ret = poll(fds.data(), fds.size(), -1);
		if(ret == -1)
		{
			std::cerr << "Error in poll()!" << std::endl;
			break;
		}
		for (size_t i = 0; i < fds.size(); ++i)
		{
			if(fds[i].revents && POLLIN) //revents tells us what event happened on each socket
			{
				if (fds[i].fd == sockfd) //new client connection
					Server::new_client();
				else //receiving data
					Server::receiving_data(i);
			}
		}
		
	}

	for (size_t i = 0; i < fds.size(); ++i)
		close(fds[i].fd);
	close(sockfd);

	//Need to handle the password authentication when a client first connects.
	//Each client will need to set a nickname and username. Need to track the client states.
	//Need to manage the channels, user roles (operator or regular user), and commands, handle channel membership and permissions.
	//When a message is sent to a channel, you’ll need to forward it to all other clients in that channel.
}

void Server::new_client()
{
	struct sockaddr_in client_addr;
	socklen_t client_size = sizeof(client_addr);
	int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_size);

	if (client_sockfd != -1)
	{
		fcntl(client_sockfd, F_SETFL, O_NONBLOCK);
		
		struct pollfd clientpoll_fd;
		clientpoll_fd.fd = client_sockfd;
		clientpoll_fd.events = POLLIN;
		fds.push_back(clientpoll_fd);
		clients.insert(std::make_pair(client_sockfd, Client(client_sockfd)));
		std::cout << "New client connected! Waiting for authentication ..." << std::endl;
		send(client_sockfd, "Please enter PASS <password> \n", 30, 0);
	}
}

void Server::receiving_data(int i)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int book = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

	if (book <= 0)
	{
		close(fds[i].fd);
		clients.erase(fds[i].fd);
		fds.erase(fds.begin() + i);
		std::cout << "The Client disconnected." << std::endl;
		--i;
	}
	Server::client_authentication(i, buffer);
	// else
	// {
	// 	Server::handle_msg(i, buffer);
	// 	// std::cout << "Received message: " << buffer << std::endl;
	// 	// send(fds[i].fd, "Message received\n", 17, 0);
	// }
}

// void Server::handle_msg(int i, std::string msg)
// {}

std::string Server::trim_p(std::string pass)
{
	size_t start = pass.find_first_not_of(" \t\n\r");
	size_t end = pass.find_last_not_of(" \t\n\r");

	if (start == std::string::npos || end == std::string::npos)
		return "";
	return (pass.substr(start, end - start + 1));
}

void Server::client_authentication(int i, std::string msg)
{
	if (clients[fds[i].fd].authenticated == false)
	{
		if (msg.find("PASS " == 0))
		{
			int	retry = 0;
			std::string pass = Server::trim_p(msg.substr(5));
			if (pass == password)
			{
				clients[fds[i].fd].authenticated = true;
				send(fds[i].fd, "Password accepted. Please enter NICK <yournickname>", 51, 0);
			}
			else
			{
				if (retry < 3)
				{
					send(fds[i].fd, "Incorrect password, please enter PASS <password> ", 49, 0);
					retry++;
				}
				else
				{
					send(fds[i].fd, "Incorrect password. Connection closing.\n", 40, 0);
    	            close(fds[i].fd);
    	            clients.erase(fds[i].fd);
    	            fds.erase(fds.begin() + i);
				}
			}
		}
		else
    	    send(fds[i].fd, "You must send PASS <password> first.\n", 38, 0);
	}
}