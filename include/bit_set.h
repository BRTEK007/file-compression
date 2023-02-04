#ifndef BIT_SET_H
#define BIT_SET_H

#include "byte_slice.h"

#include <stdlib.h>
#include <stdint.h>

typedef struct bit_set_t{
   unsigned char* bytes;
   uint64_t read_id;//id of bit to read
   uint64_t write_id;//id of bit to write
} bit_set_t;

void bit_set_init(bit_set_t* bit_set);

void bit_set_free(bit_set_t* bit_set);

void bit_set_write_bit(bit_set_t* bit_set, bool bit);

bool bit_set_read_bit(bit_set_t* bit_set);

void bit_set_write_byte(bit_set_t* bit_set);

bool bit_set_read_byte(bit_set_t* bit_set);

// void bitstack_init(bitstack_t* bs);

// void bitstack_print(bitstack_t* bs);

// void bitstack_print_format(bitstack_t* bs);

// int bitstack_get_bit(bitstack_t* bs, int index);

// unsigned char bitstack_get_8_bits(bitstack_t* bs, int index);

// void bitstack_free(bitstack_t* bs);

// void bitstack_push_one(bitstack_t* bs);

// void bitstack_push_byte(bitstack_t* bs, unsigned char byte);

// void bitstack_push_zero(bitstack_t* bs);

// void bitstack_push_bits(bitstack_t* bs, bit_code_t bits);

// void bitstack_push_bitstack(bitstack_t* bs, bitstack_t* bs2);

#endif