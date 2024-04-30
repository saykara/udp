#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define PACKET_SIZE sizeof(Message)

void udp_server()
{
  std::cout << "Size of message is " << sizeof(Message) << std::endl;
  int sockfd;
  struct sockaddr_in serv, cli;
  socklen_t addr_len = sizeof(cli);
  char buffer[PACKET_SIZE];

  // Create UDP Socket file descriptor
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    std::cerr << "Failed to create socket!" << std::endl;
    return;
  }

  // Initializing server address
  memset(&serv, 0, sizeof(serv));
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = htonl(INADDR_ANY);
  serv.sin_port = htons(PORT);

  // Bind socket address
  if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0)
  {
    std::cerr << "Failed to bind socket address!" << std::endl;
    return;
  }
  std::cout << "[UDP Server] running." << std::endl;

  uint64_t seqNum, prevNum = NULL;
  Message req, res;

  // Send & Receive
  while (true)
  {
    // Receive data from the client
    ssize_t recvBytes = recvfrom(sockfd, &buffer, PACKET_SIZE, 0, (struct sockaddr*)&cli, &addr_len);
    if (recvBytes < 0)
    {
      std::cerr << "Failed while receiving message!" << std::endl;
      break;
    }

    // Process received message
    req = deserializeMessage(buffer);
    std::cout << "Received from client: " << req.seqNum << " - " << req.data << std::endl;

    prevNum = detect_connection_failures(prevNum, req.seqNum);

    // Create response message
    Message res{ req.seqNum, "Hello Bob!" };

    // Serialize response message
    char msg[PACKET_SIZE];
    serializeMessage(res, msg);

    ssize_t sentBytes = sendto(sockfd, msg, PACKET_SIZE, 0, (struct sockaddr*)&cli, addr_len);
    if (sentBytes < 0)
    {
      std::cerr << "Failed to send message!" << std::endl;
      break;
    }
  }

  // Close the socket
  close(sockfd);
  return;
}

int main()
{
  // Run udp server
  std::cout << "[UDP Server] starting." << std::endl;
  udp_server();
  return 0;
}
