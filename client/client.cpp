#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include "checksum.cpp"

// std::string CalculateChecksum(const std::vector<char>& data, size_t size);

//STOR
void UploadFile(int clientFd, const std::string& filePath){};
//RETR
void DownloadFile(int clientFd, const std::string& fileName){};

int main(){
    std::cout << "Connected to FTP server\n";
    std::string testStr = "TEST";
    std::vector<char> data(testStr.begin(), testStr.end());
    auto result = CalculateChecksum(data, data.size());
    std::cout << "Hash result: " << result << "\n";
    return 0;
};