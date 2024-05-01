#include "client.h"


void Client::configure_udp_socket() {
  addr_len_ = sizeof(serv_);
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

  // Validate IP Address
  if (inet_pton(AF_INET, SERVER_IP, &serv_.sin_addr) <= 0) {
    throw std::runtime_error("Invalid IP address!");
  }
}

void Client::send_message(Message req) {
  // Serialize request message
  char msg[PACKET_SIZE];
  serializeMessage(req, msg);

  // Send request message to the server
  ssize_t sendBytes = sendto(sockfd_, msg, PACKET_SIZE, 0, (struct sockaddr*)&serv_, addr_len_);
  if (sendBytes < 0) {
    throw std::runtime_error("Failed to send message!");
  }
}

auto Client::receive_message() -> Message{
  // Recieve response message from the server
  ssize_t recvBytes = recvfrom(sockfd_, buffer_, PACKET_SIZE, 0, nullptr, nullptr);
  if (recvBytes < 0) {
    throw std::runtime_error("Failed while receiving message!");
  }

  // Process received message
  Message req = deserializeMessage(buffer_);
  std::cout << "Received from server | Seq: " << req.header.seqNum << " - Time: " \
            << req.header.timestamp <<  " - " << req.payload.data << std::endl;
  return req;
}

void Client::udp_client()
{
  try {
    // Configure UDP Socket
    configure_udp_socket();

    std::cout << "[UDP Client] running." << std::endl;
    uint64_t seqNum = 1, prevNum = 0;
    Message msg;

    while (true)
    {
      // Create request message
      msg.header = { seqNum, std::time(nullptr) };
      msg.payload = { "Hello Alice!" };

      // Send message
      send_message(msg);

      // Receive message
      msg = receive_message();

      // Detect communication failures
      prevNum = detect_connection_failures(prevNum, msg.header.seqNum);

      // Increment counter & wait for the specified frequency
      if (seqNum != UINT64_MAX) {
        seqNum++;
      }
      else {
        seqNum = 0;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(FREQUENCY_MS));
    }

    close(sockfd_);
    return;
    
  } catch (const std::exception& e) {
    // Catch the exception and handle it
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }
}

int main()
{
  // Run udp client
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << "[UDP Client] starting." << std::endl;

  Client cli;
  cli.udp_client();

  std::cout << "[UDP Client] exiting." << std::endl;
  return 0;
}
