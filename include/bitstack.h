#ifndef BITSTACK_H
#define BITSTACK_H

#include "bitcode.h"

typedef struct bitstack_t{
   unsigned char* bytes;
   bit_code_t bits; 
} bitstack_t;

void bitstack_init(bitstack_t* bs);

void bitstack_print(bitstack_t* bs);

void bitstack_print_format(bitstack_t* bs);

int bitstack_get_bit(bitstack_t* bs, int index);

unsigned char bitstack_get_8_bits(bitstack_t* bs, int index);

void bitstack_free(bitstack_t* bs);

void bitstack_push_one(bitstack_t* bs);

void bitstack_push_byte(bitstack_t* bs, unsigned char byte);

void bitstack_push_zero(bitstack_t* bs);

void bitstack_push_bits(bitstack_t* bs, bit_code_t bits);

void bitstack_push_bitstack(bitstack_t* bs, bitstack_t* bs2);

#endif