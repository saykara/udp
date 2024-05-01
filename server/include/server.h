#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils.h"

// Environmental variables
#define PORT std::atoi(std::getenv("PORT"))
#define BUFFER_SIZE std::atoi(std::getenv("BUFFER"))
#define SERVER_IP std::getenv("SERVER_IP")
constexpr int PACKET_SIZE = sizeof(Message);


class Server {
  public:
    // Main loop
    void udp_server();
  
  private:
    // Functions
    void configure_udp_socket();
    void send_message(Message req);
    auto receive_message() -> Message;

    // Variables
    int sockfd_;
    struct sockaddr_in serv_, cli_;
    socklen_t addr_len_;
    char buffer_[PACKET_SIZE];
};