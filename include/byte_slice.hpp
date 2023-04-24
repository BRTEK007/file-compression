#ifndef BYTE_SLICE_H
#define BYTE_SLICE_H

#define BYTE_SLICE_BIT_COUNT 8
#include <stdbool.h>
#include <stdint.h>

struct ByteSlice{
  unsigned char bits;
  int len;
  ByteSlice();
  void write_bit(bool bit);//adds bit to the end
  bool read_bit(); //pop bit from beggining
  void set_byte(unsigned char byte);
};

#endif