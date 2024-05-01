#include <algorithm>
#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cstdint>
#include <ctime>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>


static constexpr int BUFFER_SIZE = 1024;
#define PACKETSIZE sizeof(Message);


// Udp message header
struct Header {
  u_int64_t seqNum = 0;
  u_int64_t timestamp = 0;
};

// Udp message payload
struct Payload {
  char data[BUFFER_SIZE];
};

// Udp message
struct Message {
  Header header;
  Payload payload;
};

// Function to serialize the data into a character array
void serializeMessage(Message& msg, char* buffer)
{
  // Serialize sequence number in front of the array
  u_int64_t* q = (u_int64_t*)buffer;
  *q = msg.header.seqNum;
  q++;
  *q = msg.header.timestamp;
  q++;

  // Serialize message in to the array
  char* p = (char*)q;
  int i = 0;
  while (i < BUFFER_SIZE) {
    *p = msg.payload.data[i];
    p++;
    i++;
  }
}

// Function to deserialize the data from a character array
Message deserializeMessage(char* buffer)
{
  Message msg;
  uint64_t* q = (uint64_t*)buffer;

  // deserialize sequence number of the data
  msg.header.seqNum = *q;
  q++;
  msg.header.timestamp = *q;
  q++;

  // deserialize data
  char* p = (char*)q;
  int i = 0;
  while (i < BUFFER_SIZE) {
    msg.payload.data[i] = *p;
    p++;
    i++;
  }
  return msg;
}

int detect_connection_failures(uint64_t pre, uint64_t curr) {
  if (pre == UINT64_MAX) {
    return curr;
  }

  // Repeated data arrived
  if (pre == curr) {
    std::cerr << "Repeated data! Sequence no: " << curr << std::endl;
    return curr;
  }

  // Missing or out of order package
  if (pre + 1 != curr) {
    if (pre > curr) {
      std::cerr << "Data out of order! Previous: " << pre << " - Current: "<< curr << std::endl;
      return pre;
    } else {
      // Print miising packages.
      std::cerr << "Missing packages ";
      for (int i = pre + 1; i < curr; i++) {
        std::cerr << i;
        if (i == curr - 1) {
          std::cerr << std::endl;
        } else {  
          std::cerr << ", ";
        }
      }

      return curr;
    }
  }
  return curr;
}