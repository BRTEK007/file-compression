#include "bitcode.h"
#include "stdio.h"

void bit_code_print(bit_code_t bc){
  int mask = 1u;
  char str[bc.len + 1];
  for(int i = 0; i < bc.len; i++){
    str[i] = mask & bc.code ? '1' : '0';
    mask = mask << 1;
  }
  str[bc.len] = '\0';
  printf("%s", str);
}

bit_code_t bit_code_empty(){
  bit_code_t bc;
  bc.len = 0;
  bc.code = 0;
  return bc;
}

bit_code_t bit_code_add_one(bit_code_t bc){
  bit_code_t nbc = bc;
  nbc.code = nbc.code | (1u << nbc.len);
  nbc.len++;
  return nbc;
}

int bit_code_pop_msb(bit_code_t* bc){
  // int r = bc->code & (1u << (bc->len-1))?  1 : 0;
  int r = bc->code & 1u;
  bc->code = bc->code >> 1;
  bc->len--;
  return r;
}

bit_code_t bit_code_add_zero(bit_code_t bc){
  bit_code_t nbc = bc;
  nbc.len++;
  return nbc;
}