#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <openssl/sha.h>

std::string CalculateChecksum(const std::vector<char>& data, size_t size);

//STOR
void UploadFile(int clientFd, const std::string& filePath){};
//RETR
void DownloadFile(int clientFd, const std::string& fileName){};

int main(){
    std::cout << "Connected to FTP server\n";
    return 0;
};