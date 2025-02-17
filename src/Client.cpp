#include "Client.hpp"

Client::Client(int fd):sockfd(fd), authenticated(false)
{}