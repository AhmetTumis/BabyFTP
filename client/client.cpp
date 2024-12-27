#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "checksum.h"

//STOR
void UploadFile(int clientFd, const std::string& filePath){

    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "File not found: " << filePath << "\n";
        return;
    }

    std::vector<char> fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string filename = filePath.substr(filePath.find_last_of("/\\") + 1);
    std::string checksum = CalculateChecksum(fileData, fileData.size());

    std::string command = "STOR " + filename;
    send(clientFd, command.c_str(), command.size(), 0);

    send(clientFd, checksum.c_str(), checksum.size(), 0);
    send(clientFd, "\n", 1, 0); // what about \0

    send(clientFd, fileData.data(), fileData.size(), 0);
    std::cout << "File Uploaded\n";
};
//RETR
void DownloadFile(int clientFd, const std::string& fileName){
    std::string command = "RETR " + fileName;
    send(clientFd, command.c_str(), command.size(), 0);

    char checksumBuffer[65] = {0};
    read(clientFd, checksumBuffer, 64); // Receive checksum
    std::cout << "Received checksum: " << checksumBuffer << "\n";
    std::filesystem::create_directory("./received/");
    std::ofstream file("./received/" + fileName, std::ios::binary);

    std::vector<char> fileData(4096);
    ssize_t bytesReceived;

    while ((bytesReceived = read(clientFd, fileData.data(), fileData.size())) > 0) {
        file.write(fileData.data(), bytesReceived);
    }

    std::string calculatedChecksum = CalculateChecksum(fileData, fileData.size());
    if (calculatedChecksum == checksumBuffer) {
        std::cout << "File downloaded successfully with matching checksum.\n";
    } else {
        std::cerr << "Checksum mismatch for file: " << fileName << "\n";
    }
};

int main() {
    const size_t chunkSize = 4096;
    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2102);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(clientFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
    std::cout << "Connected to FTP server\n";

    while (true) {
        std::string command, filepath;
        std::cout << "ftp> ";
        std::getline(std::cin, command);

        if (command.substr(0, 4) == "LIST") {
            std::vector<char> buffer(chunkSize);
            ssize_t bytesReceived;
            send(clientFd, command.c_str(), command.size(), 0);
            std::cout << "Directory Listing:\n";
            while ((bytesReceived = read(clientFd, buffer.data(), buffer.size())) > 0) {
                std::cout.write(buffer.data(), bytesReceived);
                if (bytesReceived < static_cast<ssize_t>(chunkSize)) break;
            }
            std::cout << "\n";

        } else if (command.substr(0, 4) == "STOR") {
            std::cout << "Enter file path: ";
            std::getline(std::cin, filepath);
            UploadFile(clientFd, filepath);
        } else if (command.substr(0, 4) == "RETR") {
            std::cout << "Enter filename: ";
            std::getline(std::cin, filepath);
            DownloadFile(clientFd, filepath);
        } else {
            send(clientFd, command.c_str(), command.size(), 0);
        }
    }

    close(clientFd);
    return 0;
}
