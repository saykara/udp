cmake_minimum_required(VERSION 3.10)

project(server)

# Set C++ version to 20
set(CMAKE_CXX_STANDARD 20)

# Include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
)

# Add executable
add_executable(server src/server.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include)
