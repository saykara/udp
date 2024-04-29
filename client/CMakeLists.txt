cmake_minimum_required(VERSION 3.10)

project(client)

add_executable(client src/client.cpp)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include)