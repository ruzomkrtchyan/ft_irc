#include "Pass.hpp"

void Pass:execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    if (client.isAuth())
    {
        send(client.getFd(), "You are already authenticated.\n", 32, 0);
        return;
    }
    if (args.size() < 2)
    {
        send(client.getFd(), "Usage: PASS <password>\n", 23, 0);
        return;
    }
    std::string pass = args[1];
    retry = 0;
    if (pass == serv.getPassword())
    {
        client.authenticate();
        send(client.getFd(), "Password accepted. Please enter NICK <yournickname> \n", 53, 0);
    }
    else
	{
		if (retry < 3)
		{
			send(client.getFd(), "Incorrect password, please enter PASS <password> \n", 50, 0);
			retry++;
		}
		else
		{
			send(client.getFd(), "Incorrect password. Connection closing.\n", 40, 0);
			close(client.getFd());
			// clients.erase(fds[i].fd);
			// fds.erase(fds.begin() + i);
		}
	}


}

if (clients[fds[i].fd].authenticated == false)
	{
		if (msg.find("PASS ") == 0)
		{
			int	retry = 0;
			std::string pass = Server::trim_p(msg.substr(5));
			if (pass == password)
			{
				clients[fds[i].fd].authenticated = true;
				send(fds[i].fd, "Password accepted. Please enter NICK <yournickname> \n", 53, 0);
			}
			else
			{
				if (retry < 3)
				{
					send(fds[i].fd, "Incorrect password, please enter PASS <password> \n", 50, 0);
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