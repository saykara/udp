#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <cstdint>
#include <climits>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "utils.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define PACKET_SIZE sizeof(Message)
#define FREQUENCY_MS 1000

void udp_client()
{
  int sockfd;
  struct sockaddr_in serv;
  socklen_t addr_len = sizeof(serv);
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

  std::cout << "[UDP Client] running." << std::endl;
  int seqNum = 0;
  Message req, res;

  while (true)
  {
    // Create request message
    Message req = { seqNum, "Hello Alice!" };

    // Serialize request message
    char msg[PACKET_SIZE];
    serializeMessage(req, msg);

    // Send request message to the server
    ssize_t sendBytes = sendto(sockfd, msg, PACKET_SIZE, 0, (struct sockaddr*)&serv, addr_len);
    if (sendBytes < 0)
    {
      std::cerr << "Failed to send message!" << std::endl;
      break;
    }

    // Recieve response message from the server
    ssize_t recvBytes = recvfrom(sockfd, buffer, PACKET_SIZE, 0, nullptr, nullptr);
    if (recvBytes < 0)
    {
      std::cerr << "Failed while receiving message!" << std::endl;
      break;
    }

    // Process received message
    req = deserializeMessage(buffer);
    std::cout << "Received from server: " << req.seqNum << " - " << req.data << std::endl;

    // Increment counter & wait for the specified frequency
    if (seqNum != INT_MAX)
    {
      seqNum++;
    }
    else
    {
      seqNum = 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(FREQUENCY_MS));
  }

  close(sockfd);
  return;
}

int main()
{
  // Run udp client
  std::cout << "[UDP Client] starting." << std::endl;
  udp_client();
  return 0;
}
