cmake_minimum_required(VERSION 3.10)

project(server)

add_executable(server src/server.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include)
