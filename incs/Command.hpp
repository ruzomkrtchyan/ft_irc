#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "replice.hpp"

class Command
{
	public:
		virtual ~Command();
		virtual void execute(Server &serv, Client &client, const std::vector<std::string>& args) = 0; 
};

class Pass : public Command
{
	public:
		Pass();
		~Pass();
		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Nick : public Command
{
	public:
		Nick();
		~Nick();
		
		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class User : public Command
{
	public:
		User();
		~User();
		
		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Join : public Command
{
	public:
		Join();
		~Join();

		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Privmsg : public Command
{
	public:
		Privmsg();
		~Privmsg();

		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Invite : public Command
{
	public:
		Invite();
		~Invite();

		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Kick : public Command 
{
	public:
		Kick();
		~Kick();

		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Mode : public Command
{
    public:
        Mode();
        ~Mode();
        
        void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Topic : public Command
{
	public:
		Topic();
		~Topic();

		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Quit : public Command
{
	public:
		Quit();
		~Quit();

		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Ping : public Command
{
	public:
		Ping();
		~Ping();

		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
};

class Cap : public Command
{
	public:
		Cap(){};
		~Cap(){};

		void execute(Server &serv, Client &client, const std::vector<std::string>& args) {
			std::cout << "mtav\n";
			(void)client,(void)serv,(void)args;
		};
};

// class Pong : public Command
// {
// 	public:
// 		Pong();
// 		~Pong();

// 		void execute(Server &serv, Client &client, const std::vector<std::string>& args);
// };


#endif