#ifndef BYTE_FREQ_H
#define BYTE_FREQ_H

#include <vector>
#include <stdlib.h>

namespace oddcod
{

  struct ByteFreq
  {
    unsigned char byte;
    size_t freq;
  };

  std::vector<ByteFreq> findByteFrequencies(const std::vector<unsigned char> &bytes_in);

};

#endif