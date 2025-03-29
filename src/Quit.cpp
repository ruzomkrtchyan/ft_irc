#include "Command.hpp"

Quit::Quit(){}

Quit::~Quit(){}

void Quit::execute(Server &serv, Client &client, const std::vector<std::string>& args)
{
    std::string msg = "Client Quit\n";

    if (args.size() > 1)
    {
        if (args[1][0] == ':')
	    {
	    	msg = args[1].substr(1);
	    	for (size_t i = 2; i < args.size(); ++i)
	    		msg += " " + args[i];
	    }
	    else
	    	msg = args[1];
    }

    std::string quit_msg = ":" + client.getPrefix(client) + " QUIT :" + msg;

    for (std::map<std::string, Channel*>::iterator it = serv._channels.begin(); it != serv._channels.end(); ++it)
    {
        Channel *chn = it->second;
        if(chn->isMember(client))
        {
            chn->broadcast(quit_msg, client);
            chn->removeMember(client);
        }
    }

    serv.removeClient(client);
}