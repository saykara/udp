#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils.h"

#define PORT 8080 
#define BUFFER_SIZE 1024

void udp_server() {
    int sockfd;
    struct sockaddr_in serv, cli;
    socklen_t addr_len = sizeof(cli);
    char buffer[BUFFER_SIZE];

    // Create UDP Socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket!" << std::endl;
        return;
    }

    // Initializing server address
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    // Bind socket address
    if (bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        std::cerr << "Failed to bind socket address!" << std::endl;
        return;
    }
    std::cout << "[UDP Server] running." << std::endl;

    // Send & Receive
    while (true) {
        // Receive data from the client
        ssize_t recvBytes = recvfrom(sockfd, &buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cli, &addr_len);
        if (recvBytes < 0) {
            std::cerr << "Failed while receiving message!" << std::endl;
            break;
        }

        // Process received message
        // TODO Check communication failures: repeated data, lost data, data out of order.
        buffer[recvBytes] = '\0'; // Terminate the receiving buffer
        std::cout << "Received from client: " << buffer << std::endl;

        // Respond to the client
        const char* msg = "Message response!";
        ssize_t sentBytes = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&cli, addr_len);
        if (sentBytes < 0) {
            std::cerr << "Failed to send message!" << std::endl;
            break;
        }
    }

    // Close the socket
    close(sockfd);
    return;
}


int main() {
    // Run udp server
    std::cout << "[UDP Server] starting." << std::endl;
    udp_server();
    return 0;
}
