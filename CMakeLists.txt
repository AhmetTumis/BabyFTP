cmake_minimum_required(VERSION 3.22)

project(BabyFTP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(client client/client.cpp)
add_executable(server server/server.cpp)

#checksum
find_package(OpenSSL REQUIRED)
target_link_libraries(client OpenSSL::Crypto)
target_link_libraries(server OpenSSL::Crypto)

