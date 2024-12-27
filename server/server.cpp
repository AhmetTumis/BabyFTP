#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <filesystem>
#include <dirent.h>
#include "checksum.h"

void HandleClient(int clientSocket){

    const std::string baseDir = "./hosted_files/";
    const size_t chunkSize = 4096; //make this changable
    
    while(true){

        std::vector<char> buffer(chunkSize);
        ssize_t bytesReceived = read(clientSocket, buffer.data(), buffer.size());
        std::cout << "Message received\n";
        if(bytesReceived <= 0)
            break;
        
        std::string command(buffer.data(), bytesReceived);

        if(command.substr(0,4) == "LIST"){
            // List files in directory
            DIR* dir = opendir(baseDir.c_str());
            if (!dir) {
                std::string err = "550 Failed to open directory\n";
                send(clientSocket, err.c_str(), err.size(), 0);
            } else {
                std::string file_list;
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    if (entry->d_name[0] != '.') { // Skip hidden files
                        file_list += entry->d_name;
                        file_list += "\n";
                    }
                }
                closedir(dir);
                std::cout << "File list is: " << file_list << "\n";
                send(clientSocket, file_list.c_str(), file_list.size(), 0);
            }

        } else if(command.substr(0,4) == "RETR"){
            std::string fileName = baseDir + command.substr(5);
            std::ifstream file(fileName, std::ios::binary);

            if(!file){
                std::string err = "550 File Not Found\n";
                send(clientSocket, err.c_str(), err.size(), 0);
            } else {
                std::vector<char> fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                std::string checksum = CalculateChecksum(fileData, fileData.size());
                std::cout << "Calculated checksum for: " << command.substr(5) << "::" << checksum << "\n";
                send(clientSocket, checksum.c_str(), checksum.size(), 0);
                send(clientSocket, "\n", 1, 0);
                send(clientSocket, fileData.data(), fileData.size(), 0);
            }

        } else if (command.substr(0,4) == "STOR"){
        std::string filename = baseDir + command.substr(5);
            std::ofstream file(filename, std::ios::binary);

            char checksumBuffer[65] = {0};
            read(clientSocket, checksumBuffer, 64); // Receive checksum

            std::vector<char> fileData;
            while ((bytesReceived = read(clientSocket, buffer.data(), buffer.size())) > 0) {
                fileData.insert(fileData.end(), buffer.begin(), buffer.begin() + bytesReceived);
                if (bytesReceived < static_cast<ssize_t>(chunkSize))
                    break;
            }

            std::string calculatedChecksum = CalculateChecksum(fileData, fileData.size());
            if (calculatedChecksum == checksumBuffer) {
                file.write(fileData.data(), fileData.size());
                std::string msg = "200 OK\n";
                send(clientSocket, msg.c_str(), msg.size(), 0);
            } else {
                std::string msg = "550 Checksum Mismatch\n";
                send(clientSocket, msg.c_str(), msg.size(), 0);
            }
        } else {
            std::string msg = "500 Unknown Command\n";
            send(clientSocket, msg.c_str(), msg.size(), 0);
        }

    }
};

int main(){

    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2102);
    serverAddr.sin_len = INADDR_ANY;
    bind(serverFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverFd, 1);
    std::cout << "FTP server listening on port 2102...\n";

    int client_socket = accept(serverFd, nullptr, nullptr);
    HandleClient(client_socket);

    close(client_socket);
    close(serverFd);
    return 0;
}