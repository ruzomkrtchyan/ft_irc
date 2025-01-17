
#include <iostream>
#include <sys/socket.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // creates an endpoint for communication
    if (sockfd == -1)
    {
        std::cerr << "Failed to create a socket!" << std::endl;
        return(-1); 
    }
    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr);

    if (bind(sockfd, AF_INET, &serv_addr, sizeof(serv_addr)) == -1)
    {
        std::cerr << "Failed to bind to IP/Port!" << std::endl;
        return(-1);
    }
    if (listen(sockfd, SOMAXCONN) == -1)
    {
        std::cerr << "Failed to listen!" << std::endl;
        return(-1);
    }
}