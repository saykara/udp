#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define FREQUENCY_MS 1000

void udp_client() {
    int sockfd;
    struct sockaddr_in serv;
    socklen_t addr_len = sizeof(serv);
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

    int ctr = 0;
    std::string msg;
    std::cout << "[UDP Client] running." << std::endl;
    while (true) {
        // Create request message
        msg = std::to_string(ctr);

        // Send request message to the server
        ssize_t sendBytes = sendto(sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr *)&serv, addr_len);
        if (sendBytes < 0) {
            std::cerr << "Failed to send message!" << std::endl;
            break;
        }

        // Recieve response message from the server
        ssize_t recvBytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, nullptr, nullptr);
        if (recvBytes < 0) {
            std::cerr << "Failed while receiving message!" << std::endl;
            break;
        }

        // Process received message
        // TODO Check communication failures: repeated data, lost data, data out of order.
        buffer[recvBytes] = '\0'; // Terminate the receiving buffer
        std::cout << "Received from client: " << buffer << std::endl;

        // Increment counter & wait for the specified frequency
        if (ctr != UINT64_MAX) {
            ctr++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(FREQUENCY_MS));
    }

    close(sockfd);
    return;
}

int main() {
    // Run udp client
    std::cout << "[UDP Client] starting." << std::endl;
    udp_client();
    return 0;
}
