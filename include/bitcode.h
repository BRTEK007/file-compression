#ifndef BITCODE_H
#define BITCODE_H

typedef struct bit_code_t{
  unsigned char code;
  int len;
} bit_code_t;

void bit_code_print(bit_code_t bc);

bit_code_t bit_code_empty();

bit_code_t bit_code_add_one(bit_code_t bc);

int bit_code_pop_msb(bit_code_t* bc);

bit_code_t bit_code_add_zero(bit_code_t bc);

#endif