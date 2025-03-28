#include "Server.hpp"
#include "Manager.hpp"

Server::Server(int prt, std::string passw) : port(prt), password(passw)
{
	Server::create_sock();
	cmdManager = new Manager();
}

Server::~Server()
{
	delete cmdManager;
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
		if(clients.find(fds[i].fd) != clients.end())
		{
			Client &client = clients[fds[i].fd];
			std::cout << "The Client disconnected:" << client.getNickname() <<std::endl;

			removeClient(client);
		}

		fds.erase(fds.begin() + i);
		return;
	}
	std::string msg(buffer);
	Client &client = clients[fds[i].fd];
	Server::handle_msg(client, msg);
}

std::vector<std::string> splitCommand(const std::string &command)
{
	std::vector<std::string> args;
	std::stringstream ss(command);
	std::string token;

	while (ss >> token)
		args.push_back(token);
	return args;
}

void Server::handle_msg(Client &client, std::string msg)
{
	std::vector<std::string> args = splitCommand(msg);
	if (args.empty()) return;

	std::string cmd = args[0];
	cmdManager->execute(cmd, *this, client, args);
}

std::string	Server::getPassword()
{
	return password;
}

Client* Server::get_client_bynick(std::string &nick)
{
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getNickname() == nick)
			return &it->second;
	}
	return NULL;
}

Channel* Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

Channel* Server::createChannel(const std::string& name, Client& creator)
{
	Channel* newChannel = new Channel(name, creator);
	_channels[name] = newChannel;
	return newChannel;
}

void Server::removeClient(Client& client) 
{
    int fd = client.getFd();

    close(fd);
    for (size_t i = 0; i < fds.size(); i++)
	{
        if (fds[i].fd == fd) 
		{
            fds.erase(fds.begin() + i);
            break;
        }
    }
    clients.erase(fd);
    std::cout << "Client with FD " << fd << " removed." << std::endl;
}

void Server::checkForClosedChannels()
{
    std::map<std::string, Channel*>::iterator it = _channels.begin();
    while (it != _channels.end())
    {
        if (it->second->_members.empty())  // If no users left in the channel
        {
            delete it->second;  // Free memory
            	_channels.erase(it);  // Remove from map and move iterator
        }
        else
            ++it;
    }
}

// void Server::leaveAllChannels(Client& client)
// {
//     for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
//         removeClient(client);
//     _channels.clear();
// }

bool Server::isClientTaken(const std::string& targetNickname) {
    for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.getNickname() == targetNickname) {
            return true; // Client with this nickname exists
        }
    }
    return false; // No client found with this nickname
}