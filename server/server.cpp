#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include "checksum.cpp"


void HandleClient(int clientSocket){};

int main(){
    std::cout << "FTP server listening\n";
    std::string testStr = "TEST";
    std::vector<char> data(testStr.begin(), testStr.end());
    auto result = CalculateChecksum(data, data.size());
    std::cout << "Hash result: " << result << "\n";
    return 0;
}