#ifndef DECOMPRESS_H
#define DECOMPRESS_H
#include <vector>
#include <ostream>

void decompress(const std::vector<unsigned char>& in_buffer, std::ostream& outStream);

#endif