#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstring>

#define PORT 8081 // Change if needed
#define MAX_CONNECTION 10 // For test purpose

int main() {
    int sockfd, con;
    int opt = 1;
    struct sockaddr_in address{};
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char const *serv_message = "Hello World! From server.";

    // AF_INET -> IPV4 | SOCK_STREAM -> TCP | 0 -> Internet Protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set to socket, options at level SOL_SOCKET (SO_REUSEADDR to avoid errno 98)
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET; // IPV4
    address.sin_addr.s_addr = INADDR_ANY; // Accept any entry address
    address.sin_port = htons(PORT); // 8081 in this case

    // Associate IP addr and port number to socket
    if(bind(sockfd, (struct sockaddr*)&address, addrlen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Socket listening on port 8081 (defined before)
    if(listen(sockfd, MAX_CONNECTION) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incomming connection
    if((con
        = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen))
        < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    // Read incomming message
    read(con, buffer, 1024);
    std::cout << buffer << std::endl;

    // Send server message
    send(con, serv_message, strlen(serv_message), 0);
    std::cout << "Serv message sent" << std::endl;

    // Close connection
    close(con);
    // Shutdown socket
    shutdown(sockfd, SHUT_RDWR);

    exit(EXIT_SUCCESS);
}
