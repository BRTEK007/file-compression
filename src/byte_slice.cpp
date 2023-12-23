#include "byte_slice.hpp"
#include <stdio.h>

ByteSlice::ByteSlice()
{
  len = 0;
  bits = 0;
}

void ByteSlice::write_bit(bool bit)
{
  bits = bits << 1; // shift left
  if (bit)          // place bit at the end
    bits = bits | 1u;
  len++;
}

bool ByteSlice::read_bit()
{
  len--;
  bool bit = bits & (1u << len);
  return bit;
}

void ByteSlice::set_byte(unsigned char byte)
{
  bits = byte;
  len = BYTE_SLICE_BIT_COUNT;
}