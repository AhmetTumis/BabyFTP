cmake_minimum_required(VERSION 3.22)

project(BabyFTP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/shared)

if(APPLE)
    message(STATUS "Using CommonCrypto for checksum (macOS)")
    add_library(util SHARED shared/checksum_mac.cpp)
elseif(UNIX)
    find_package(OpenSSL REQUIRED)
    message(STATUS "Using OpenSSL for checksum (Linux)")
    add_library(util SHARED shared/checksum_linux.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

add_executable(client client/client.cpp)
add_executable(server server/server.cpp)

target_link_libraries(client util)
target_link_libraries(server util)

if(UNIX AND NOT APPLE)
    target_link_libraries(client OpenSSL::Crypto)
    target_link_libraries(server OpenSSL::Crypto)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/server/hosted_files
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
