#ifndef BYTE_FREQ_H
#define BYTE_FREQ_H

#include <vector>

struct byte_freq_t{
  unsigned char byte;
  int freq;
};

std::vector<byte_freq_t> get_byte_frequencies(const std::vector<unsigned char>& bytes_in); 

#endif