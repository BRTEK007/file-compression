#include "byte_slice.hpp"
#include <stdio.h>

void byte_slice_print(byte_slice_t slice){
  while(slice.len > 0){
    printf("%d", byte_slice_read_bit(&slice));
  }
}

void byte_slice_init(byte_slice_t* byte_slice){
  byte_slice->len = 0;
  byte_slice->bits = 0;
}

void byte_slice_write_bit(byte_slice_t* byte_slice, bool bit){
  byte_slice->bits = byte_slice->bits << 1;//shift left
  if(bit)//place bit at the end
    byte_slice->bits = byte_slice->bits | 1u;
  byte_slice->len++;
}

bool byte_slice_read_bit(byte_slice_t* byte_slice){
  byte_slice->len--;
  bool bit = byte_slice->bits & (1u << byte_slice->len);
  return bit;
}