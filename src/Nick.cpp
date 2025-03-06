
void Nick::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
	if (!client.isAuth())
	{
		send(client.getFd(), "You must authenticate first with PASS.\n", 40, 0);
		return;
	}
	if (args.size() > 1)
	{
		client.setNickname(args[1]); //should set clients.nickname also
		send(client.getFd(), "Nickname set successfully! Welcome!\n", 36, 0);
		std::cout << "Client " << client.sockfd << " set nickname: " << nickname << std::endl;
	}
	else
		send(client.getFd(), "Invalid nickname. Please try again.\n", 35, 0);
	// else
	// 	send(client.sockfd, "You must set your nickname using NICK <name>\n", 44, 0);
}

// void Server::client_nickname(int i, std::string msg)
// {
// 	int client_fd = fds[i].fd;

// 	if (msg.find("NICK ") == 0)
// 	{
// 		std::string nickname = Server::trim_p(msg.substr(5));
// 		if (!nickname.empty())
// 		{
// 			clients[client_fd].nickname = nickname;
// 			send(client_fd, "Nickname set successfully! Welcome!\n", 36, 0);
// 			std::cout << "Client " << client_fd << " set nickname: " << nickname << std::endl;
// 		}
// 		else
// 			send(client_fd, "Invalid nickname. Please try again.\n", 35, 0);
// 	}
// 	else
// 		send(client_fd, "You must set your nickname using NICK <name>\n", 44, 0);
// }	