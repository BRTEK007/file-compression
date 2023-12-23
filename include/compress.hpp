#ifndef COMPRESS_H
#define COMPRESS_H

#include <vector>
#include <ostream>

void compress(const std::vector<unsigned char> &in_buffer, std::ostream &outStream);

#endif