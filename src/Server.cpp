#include "Server.hpp"

Server::Server(int prt, std::string passw) : port(prt), password(passw) 
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
	serv_fd.events = POLLIN;
	fds.push_back(serv_fd);

	while (true) 
	{
		int ret = poll(fds.data(), fds.size(), -1);
		if (ret == -1) 
		{
			std::cerr << "Error in poll()!" << std::endl;
			break;
		}

		for (size_t i = 0; i < fds.size(); ++i) 
		{
			if (fds[i].revents & POLLIN) 
			{
				if (fds[i].fd == sockfd)
					Server::new_client();
				else
					Server::receiving_data(i);
			}
		}
	}

	for (size_t i = 0; i < fds.size(); ++i)
		close(fds[i].fd);
	close(sockfd);
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

		clients.insert(std::make_pair(client_sockfd, Client(client_sockfd, client_addr)));

		std::cout << "New client connected! Waiting for authentication ..." << std::endl;
		send(client_sockfd, "Please enter PASS <password>\n", 30, 0);
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
		return;
	}
	// Server::client_authentication(i, buffer);
	// else
	// {
	// 	Server::handle_msg(i, buffer);
	// 	// std::cout << "Received message: " << buffer << std::endl;
	// 	// send(fds[i].fd, "Message received\n", 17, 0);
	// }
	std::string msg(buffer);
	Client &client = clients[fds[i].fd];
	if (!client.authenticated)
		Server::client_authentication(i, msg);
	else if (client.nickname.empty())
		Server::client_nickname(i, msg);
	else if (client.username.empty())
		Server::client_username(i, msg);
	else
		Server::processCommand(client, msg);
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
		if (msg.find("PASS ") == 0)
		{
			int	retry = 0;
			std::string pass = Server::trim_p(msg.substr(5));
			if (pass == password)
			{
				clients[fds[i].fd].authenticated = true;
				send(fds[i].fd, "Password accepted. Please enter NICK <yournickname>\n", 52, 0);
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

void Server::client_nickname(int i, std::string msg)
{
	int client_fd = fds[i].fd;

	if (msg.find("NICK ") == 0)
	{
		std::string nickname = Server::trim_p(msg.substr(5));
		if (!nickname.empty())
		{
			clients[client_fd].nickname = nickname;
			send(client_fd, "Nickname set successfully! Welcome!\n", 36, 0);
			std::cout << "Client " << client_fd << " set nickname: " << nickname << std::endl;
		}
		else
		{
			send(client_fd, "Invalid nickname. Please try again.\n", 35, 0);
		}
	}
	else
	{
		send(client_fd, "You must set your nickname using NICK <name>\n", 44, 0);
	}
}	

std::vector<std::string> splitCommand(const std::string &command)
{
	std::vector<std::string> args;
	std::stringstream ss(command);
	std::string token;

	while (ss >> token)
	{
		args.push_back(token);
	}
	return args;
}

void Server::processCommand(Client &client, const std::string &command)
{
	std::vector<std::string> args = splitCommand(command);
	if (args.empty()) return;

	if (args[0] == "NICK")
		client_nickname(client.getFd(), args[1]);
	else if (args[0] == "USER")
		client_username(client.getFd(), args[1]);
	else if (!client.isFullyRegistered()){
		send(client.getFd(), "451 :You have not registered\r\n", 32, 0);
	} else {
		// Handle other commands here
	}
}

void Server::client_username(int i, std::string msg)
{
    int client_fd = fds[i].fd;

    if (msg.find("USER ") == 0)
    {
        std::string params = msg.substr(5);
        size_t first_space = params.find(' ');
        size_t second_space = params.find(' ', first_space + 1);
        size_t third_space = params.find(' ', second_space + 1);

        if (first_space == std::string::npos || second_space == std::string::npos || third_space == std::string::npos)
        {
            send(client_fd, "ERROR: Invalid USER format. Use USER <username> 0 * <realname>\n", 61, 0);
            return;
        }

        clients[client_fd].username = params.substr(0, first_space);
        clients[client_fd].realname = params.substr(third_space + 1);

        if (!clients[client_fd].nickname.empty())
        {
            send(client_fd, "Welcome to the IRC Server!\n", 28, 0);
            std::cout << "User registered: " << clients[client_fd].nickname << " (" << clients[client_fd].username << ")" << std::endl;
        }
        else
        {
            send(client_fd, "USER info received. Now enter your nickname using NICK <name>\n", 59, 0);
        }
    }
    else
    {
        send(client_fd, "You must send USER <username> 0 * <realname>\n", 45, 0);
    }
}
