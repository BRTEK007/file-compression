#ifndef BYTE_SLICE_H
#define BYTE_SLICE_H

#define BYTE_SLICE_BIT_COUNT 8
#include <stdbool.h>
#include <stdint.h>

//array of bits, slice of a byte, 
typedef struct byte_slice_t{
  unsigned char bits;
  int len;
} byte_slice_t;

//sets len to 0, and bits to zeros
void byte_slice_init(byte_slice_t* byte_slice);

void byte_slice_print(byte_slice_t slice);

//adds bit to the end
void byte_slice_write_bit(byte_slice_t* byte_slice, bool bit);

//pop bit from beggining
bool byte_slice_read_bit(byte_slice_t* byte_slice);

#endif