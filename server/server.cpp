#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <openssl/sha.h>

std::string CalculateChecksum(const std::vector<char>& data, size_t size);

void HandleClient(int clientSocket){};

int main(){
    std::cout << "FTP server listening\n";
    return 0;
}