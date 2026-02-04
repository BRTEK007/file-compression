#ifndef BYTE_FREQ_H
#define BYTE_FREQ_H

#include <vector>
#include <stdlib.h>
#include "bit_istream.hpp"
#include "oddcod.hpp"

namespace oddcod
{
  // Holds byte value and number of occurrences.
  struct ByteFreq
  {
    word_t byte;
    size_t freq;
  };

  // Returns an array of byte frequencies derived from the input.
  std::vector<ByteFreq> findByteFrequencies(std::shared_ptr<AlignedReader> input);
};

#endif