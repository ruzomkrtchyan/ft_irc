#include <iostream>
#include <sys/socket.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // creates an endpoint for communication
    if(sockfd == -1)
    {
        std::cerr << "Failed to create a socket!" << std::endl;
        return(-1); 
    }
    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

}