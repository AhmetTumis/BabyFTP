#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <vector>

std::string CalculateChecksum(const std::vector<char>& data, size_t size){
    unsigned char hash[SHA256_DIGEST_LENGTH]; //32
    SHA256(reinterpret_cast<const unsigned char*>(data.data()), size, hash);

    std::ostringstream result;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i){
        result << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return result.str();
}