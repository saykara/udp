#include <iostream>
#include <cstring>
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
