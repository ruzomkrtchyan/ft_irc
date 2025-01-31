
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return(1);
	}

	Server serv(std::atoi(argv[1]), argv[2]);
}