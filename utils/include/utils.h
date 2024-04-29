#include <iostream>
#include <cstring>

// Udp message 
struct Message {
    int sequenceNumber;
    std::string message;
};

// Function to serialize the data into a character array
void serializeMessage(const Message& msg, char* buffer, size_t bufferSize) {
    memcpy(buffer, &msg.sequenceNumber, sizeof(int));
    
    // Copy message string into buffer, taking into account buffer size
    size_t messageSize = std::min(bufferSize - sizeof(int), msg.message.size());
    memcpy(buffer + sizeof(int), msg.message.c_str(), messageSize);
}

// Function to deserialize the data from a character array
Message deserializeMessage(const char* buffer, size_t bufferSize) {
    Message msg;
    
    // Extract sequence number from buffer
    memcpy(&msg.sequenceNumber, buffer, sizeof(int));
    
    // Extract message string from buffer
    size_t messageSize = bufferSize - sizeof(int);
    char* messageBuffer = new char[messageSize + 1];
    memcpy(messageBuffer, buffer + sizeof(int), messageSize);
    messageBuffer[messageSize] = '\0'; // End of a string
    msg.message = messageBuffer;
    delete[] messageBuffer;
    
    return msg;
}