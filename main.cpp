
#include <iostream>
#include <sys/socket.h>
#include <cstdlib>

int main(int argc, char **argv)
{
    int port_n = std::atoi(argv[1]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // creates an endpoint for communication
    if (sockfd == -1)
    {
        std::cerr << "Failed to create a socket!" << std::endl;
        return(-1); 
    }

    // Need to to set the socket to non-blocking
    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_n);
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr);

    if (bind(sockfd, AF_INET, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        std::cerr << "Failed to bind a socket!" << std::endl;
        close(sockfd);
        return(-1);
    }
    if (listen(sockfd, SOMAXCONN) == -1)
    {
        std::cerr << "Failed to listen!" << std::endl;
        close(sockfd);
        return(-1);
    }

    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);

    int client_sockfd = accept(sockfd, (struct sockaddr*)&serv_addr, &client_size);
    if (client_sockfd == -1)
    {
        std::cerr << "Failed to accept client connection!" << std::endl;
        close(sockfd);
        return(-1);
    }
    close(sockfd);
}