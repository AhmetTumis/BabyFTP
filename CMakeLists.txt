cmake_minimum_required(VERSION 3.22)

project(BabyFTP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/shared)
add_library(util SHARED shared/checksum.cpp)

add_executable(client client/client.cpp)
add_executable(server server/server.cpp)

#checksum
find_package(OpenSSL REQUIRED)
target_link_libraries(client OpenSSL::Crypto util)
target_link_libraries(server OpenSSL::Crypto util)