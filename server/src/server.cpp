#include "server.h"

void Server::configure_udp_socket() {
  addr_len_ = sizeof(cli_);
  // Create UDP Socket file descriptor
  sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd_ < 0) {
    throw std::runtime_error("Failed to create socket!");
  }

  // Initializing server address
  memset(&serv_, 0, sizeof(serv_));
  serv_.sin_family = AF_INET;
  serv_.sin_addr.s_addr = inet_addr(SERVER_IP);
  serv_.sin_port = htons(PORT);

  // Bind socket address
  if (bind(sockfd_, (struct sockaddr*)&serv_, sizeof(serv_)) < 0) {
    throw std::runtime_error("Failed to bind socket address!");
  }
}

auto Server::receive_message() -> Message {
  // Receive data from the client
  ssize_t recvBytes = recvfrom(sockfd_, &buffer_, PACKET_SIZE, 0, (struct sockaddr*)&cli_, &addr_len_);
  if (recvBytes < 0) {
    throw std::runtime_error("Failed while receiving message!");
  }

  // Process received message
  Message req = deserializeMessage(buffer_);
  std::cout << "Received from client: " << req.seqNum << " - " << req.data << std::endl;
  return req;
}

void Server::send_message(Message req) {
  // Serialize response message
  char msg[PACKET_SIZE];
  serializeMessage(req, msg);

  ssize_t sentBytes = sendto(sockfd_, msg, PACKET_SIZE, 0, (struct sockaddr*)&cli_, addr_len_);
  if (sentBytes < 0) {
    throw std::runtime_error("Failed to send message!");
  }
}

void Server::udp_server()
{
  try {
    // Configure UDP Server
    configure_udp_socket(); 

    std::cout << "[UDP Server] running." << std::endl;
    uint64_t prevNum = 0;
    Message req, res; 

    // Send & Receive
    while (true)
    {
      // Receive message from client
      req = receive_message();  

      // Detect failures
      prevNum = detect_connection_failures(prevNum, req.seqNum);  

      // Create response message
      res = { req.seqNum, "Hello Bob!" };  

      // Send response
      send_message(res);
    } 

    // Close the socket
    close(sockfd_);
    return;

  } catch (const std::exception& e) {
    // Catch the exception and handle it
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }

}

int main()
{
  // Run udp server
  std::cout << "[UDP Server] starting." << std::endl;

  Server ser;
  ser.udp_server();

  std::cout << "[UDP Server] exiting." << std::endl;

  return 0;
}
