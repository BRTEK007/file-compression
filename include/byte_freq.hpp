#ifndef BYTE_FREQ_H
#define BYTE_FREQ_H

#include <vector>

typedef struct byte_freq_t{
  unsigned char byte;
  int freq;
} byte_freq_t;

int cmp_byte_freq (const void * a, const void * b); 

std::vector<byte_freq_t> get_byte_freq_arr(std::vector<unsigned char> input); 

#endif