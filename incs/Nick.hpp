#ifndef NICK_HPP
#define NICK_HPP

class Nick : public Command
{
	public:
		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

#endif