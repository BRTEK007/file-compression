#ifndef BYTE_FREQ_H
#define BYTE_FREQ_H

#include <vector>

struct ByteFreq
{
  unsigned char byte;
  int freq;
};

std::vector<ByteFreq> findByteFrequencies(const std::vector<unsigned char> &bytes_in);

#endif