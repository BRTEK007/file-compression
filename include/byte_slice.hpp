#ifndef BYTE_SLICE_H
#define BYTE_SLICE_H

#define BYTE_SLICE_BIT_COUNT 8
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

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
};

#endif