#include "checksum.h"
#include <CommonCrypto/CommonDigest.h>

std::string CalculateChecksum(const std::vector<char>& data, size_t size) {
    unsigned char hash[CC_SHA256_DIGEST_LENGTH]; // 32

    CC_SHA256(data.data(), static_cast<CC_LONG>(size), hash);

    std::ostringstream result;
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; ++i) {
        result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return result.str();
}
