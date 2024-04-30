#include <iostream>
#include <cstring>
#include <climits>
#include <cstdint>
#include <cmath>
#include <vector>
#include <algorithm>

static constexpr int BUFFER_SIZE = 1024;
#define PACKETSIZE sizeof(Message);

// Udp message
struct Message
{
  int seqNum = 0;
  char data[BUFFER_SIZE];
};

// Function to serialize the data into a character array
void serializeMessage(Message& msg, char* buffer)
{
  // Serialize sequence number in front of the array
  int* q = (int*)buffer;
  *q = msg.seqNum;
  q++;

  // Serialize message in to the array
  char* p = (char*)q;
  int i = 0;
  while (i < BUFFER_SIZE)
  {
    *p = msg.data[i];
    p++;
    i++;
  }
}

// Function to deserialize the data from a character array
Message deserializeMessage(char* buffer)
{
  Message msg;
  int* q = (int*)buffer;

  // deserialize sequence number of the data
  msg.seqNum = *q;
  q++;

  // deserialize data
  char* p = (char*)q;
  int i = 0;
  while (i < BUFFER_SIZE)
  {
    msg.data[i] = *p;
    p++;
    i++;
  }
  return msg;
}

int detect_connection_failures(uint64_t pre, uint64_t curr) {
  // First package arrived
  if (!pre) {
    return curr;
  }

  if (pre == UINT64_MAX) {
    return 0;
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