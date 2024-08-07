#ifndef BYTE_FREQ_H
#define BYTE_FREQ_H

#include <vector>
#include <stdlib.h>
#include "bit_istream.hpp"

namespace oddcod
{

  struct ByteFreq
  {
    unsigned char byte;
    size_t freq;
  };

  std::vector<ByteFreq> findByteFrequencies(std::shared_ptr<AlignedReader> input);
};

#endif