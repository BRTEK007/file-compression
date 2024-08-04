#include "byte_slice.hpp"
namespace oddcod
{
  ByteSlice::ByteSlice() : bits(0), len(0) {}

  void ByteSlice::reset()
  {
    bits = 0;
    len = 0;
  }

  void ByteSlice::writeBit(bool bit)
  {
    bits = bits << 1; // shift left
    if (bit)          // place bit at the end
      bits = bits | 1u;
    len++;
  }

  bool ByteSlice::readBit()
  {
    len--;
    bool bit = bits & (1u << len);
    return bit;
  }

  void ByteSlice::setByte(unsigned char byte)
  {
    bits = byte;
    len = BYTE_SLICE_BIT_COUNT;
  }

  unsigned char ByteSlice::getByte()
  {
    return bits;
  }

  size_t ByteSlice::size()
  {
    return len;
  };

  bool ByteSlice::full()
  {
    return len == BYTE_SLICE_BIT_COUNT;
  }
};