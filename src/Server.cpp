#include "Server.hpp"

Server::Server(int prt, std::string passw) : port(prt), password(passw) {
    Server::create_sock();
}

void Server::create_sock() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create a socket!" << std::endl;
        exit(1);
    }

    // Set socket to non-blocking mode
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "Failed to set socket to non-blocking mode!" << std::endl;
        close(sockfd);
        exit(1);
    }

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cerr << "Failed to bind the socket!" << std::endl;
        close(sockfd);
        exit(1);
    }
    if (listen(sockfd, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen on the socket!" << std::endl;
        close(sockfd);
        exit(1);
    }
}

void Server::connect() {
    struct pollfd serv_fd;
    serv_fd.fd = sockfd;
    serv_fd.events = POLLIN;
    fds.push_back(serv_fd);

    while (true) {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1) {
            std::cerr << "Error in poll()!" << std::endl;
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) { // Corrected bitwise AND
                if (fds[i].fd == sockfd)
                    Server::new_client();
                else
                    Server::receiving_data(i);
            }
        }
    }

    for (size_t i = 0; i < fds.size(); ++i)
        close(fds[i].fd);
    close(sockfd);
}

void Server::new_client() {
    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_size);

    if (client_sockfd == -1) {
        std::cerr << "Failed to accept a client!" << std::endl;
        return;
    }

    // Set client socket to non-blocking
    if (fcntl(client_sockfd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "Failed to set client socket to non-blocking mode!" << std::endl;
        close(client_sockfd);
        return;
    }

    // Ask for password
    const char* request_msg = "Enter password:\n";
    send(client_sockfd, request_msg, strlen(request_msg), 0);

    // Receive password
    char buffer[256];
    std::memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received <= 0) {
        std::cerr << "Client failed to send a password." << std::endl;
        close(client_sockfd);
        return;
    }

    // Remove trailing newline characters
    std::string received_password(buffer);
    // Remove trailing newline characters
	received_password.erase(std::remove(received_password.begin(), received_password.end(), '\n'), received_password.end());
	received_password.erase(std::remove(received_password.begin(), received_password.end(), '\r'), received_password.end());



    // Validate password
    if (received_password != password) {
        const char* error_msg = "Incorrect password. Connection closed.\n";
        send(client_sockfd, error_msg, strlen(error_msg), 0);
        close(client_sockfd);
        return;
    }

    // Add client to poll list
    struct pollfd clientpoll_fd;
    clientpoll_fd.fd = client_sockfd;
    clientpoll_fd.events = POLLIN;
    fds.push_back(clientpoll_fd);

    std::cout << "Client authenticated successfully!" << std::endl;
    send(client_sockfd, "Welcome to the server!\n", 23, 0);
}

void Server::receiving_data(int i) {
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received <= 0) {
        std::cout << "Client disconnected." << std::endl;
        close(fds[i].fd);
        fds.erase(fds.begin() + i);
    } else {
        std::cout << "Received message: " << buffer << std::endl;
        send(fds[i].fd, "Message received\n", 17, 0);
    }
}
