#include "Command.hpp"

Nick::Nick()
{}

Nick::~Nick()
{}

bool isValidNickname(const std::string& nickname)
{
	if (nickname.empty() || nickname.length() > 9)
		return false;

	if (!std::isalpha(nickname[0]))
		return false;

	for (std::size_t i = 0; i < nickname.length(); ++i)
	{
		char c = nickname[i];
		if (!std::isalnum(c) && c != '-' && c != '_')
			return false;
	}

	return true;
}

void Nick::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    (void)serv;

    if (!client.isAuth())
    {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }

    if (args.size() < 2 || args[1].empty())
    {
        client.sendMessage(ERR_NONICKNAMEGIVEN(client.getNickname()));
        return;
    }

    std::string newNick = args[1];

    if (serv.isClientTaken(newNick))
    {
        client.sendMessage(ERR_NICKNAMEINUSE(client.getNickname(), newNick));
        return;
    }

    if (!isValidNickname(newNick))
    {
        client.sendMessage(ERR_ERRONEUSNICKNAME(client.getNickname(), newNick));
        return;
    }

    if (client.isFullyRegistered())  
    {
        std::string oldNick = client.getNickname();
        client.setNickname(newNick);
        std::string message = ":" + oldNick + " NICK " + newNick + "\r\n";
    }
    else
        client.setNickname(newNick);

    std::cout << "Client " << client.getFd() << " set nickname: " << client.getNickname() << std::endl;
}
