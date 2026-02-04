#ifndef BYTE_SLICE_H
#define BYTE_SLICE_H

#include <stdlib.h>

namespace oddcod
{
#define BYTE_SLICE_BIT_COUNT 8

  // Read and write to a byte by individual bits or full byte.
  class ByteSlice
  {
    unsigned char bits;
    size_t len;

  public:
    ByteSlice();
    void writeBit(bool bit); // adds bit to the end
    bool readBit();          // pop bit from beggining
    void setByte(unsigned char byte);
    unsigned char getByte();
    size_t size();
    bool full();
    void reset();
  };
};

#endif