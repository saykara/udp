cmake_minimum_required(VERSION 3.10)

project(client)

# Set C++ version to 20
set(CMAKE_CXX_STANDARD 20)

# Include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
)

# Add executable
add_executable(client src/client.cpp)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include)