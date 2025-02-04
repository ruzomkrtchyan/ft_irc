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

	// fcntl(client_sockfd, F_SETFL, O_NONBLOCK);
	 if (client_sockfd == -1)
	{
		std::cerr << "Failed to accept a client!" << std::endl;
		return;
	}

	// Set client socket to non-blocking
	fcntl(client_sockfd, F_SETFL, O_NONBLOCK);

	// Ask for a password
	const char* request_msg = "Enter password:\n";
	send(client_sockfd, request_msg, strlen(request_msg), 0);

	// Receive the password
	char buffer[256];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes_received = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received <= 0) 
	{
		std::cerr << "Client failed to send a password." << std::endl;
		close(client_sockfd);
		return;
	}

	// Remove trailing newline characters (clients often send passwords with '\n' or '\r')
	std::string received_password(buffer);
	received_password.erase(std::remove(received_password.begin(), received_password.end(), '\n'), received_password.end());
	received_password.erase(std::remove(received_password.begin(), received_password.end(), '\r'), received_password.end());

	// Check if the password is correct
	if (received_password != password)
	{
		const char* error_msg = "Incorrect password. Connection closed.\n";
		send(client_sockfd, error_msg, strlen(error_msg), 0);
		close(client_sockfd);
		return;
	}

	// If correct, add the client to poll list
	struct pollfd clientpoll_fd;
	clientpoll_fd.fd = client_sockfd;
	clientpoll_fd.events = POLLIN;
	fds.push_back(clientpoll_fd);

	std::cout << "Client authenticated successfully!" << std::endl;
	send(client_sockfd, "Welcome to the server!\n", 23, 0);
}

void Server::receiving_data(int i)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int book = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

	if (book <= 0)
	{
		std::cout << "The Client disconnected." << std::endl;
		close(fds[i].fd);
		fds.erase(fds.begin() + i);
		--i;
	}
	else
	{
		std::cout << "Received message: " << buffer << std::endl;
		send(fds[i].fd, "Message received\n", 17, 0);
	}
}