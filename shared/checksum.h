#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

std::string CalculateChecksum(const std::vector<char>& data, size_t size);

#endif //CHECKSUM_H