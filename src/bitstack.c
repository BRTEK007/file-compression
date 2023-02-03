#include "bitstack.h"

#include "c-vector/cvector.h"
#include <stdio.h>

void bitstack_init(bitstack_t* bs){
    cvector_vector_type(unsigned char) bytes = NULL;
    bs->bytes = bytes;
    bs->bits = bit_code_empty();
}

void bitstack_print(bitstack_t* bs){
    for(int i = 0; i < cvector_size(bs->bytes); i++){
        bit_code_t bc = bit_code_empty();
        bc.code = bs->bytes[i];
        bc.len = 8;
        bit_code_print(bc);
        printf(" ");
    }

    bit_code_print(bs->bits);
}

void bitstack_free(bitstack_t* bs){
    cvector_free(bs->bytes);
}

void bitstack_push_one(bitstack_t* bs){

}

void bitstack_push_zero(bitstack_t* bs){

}

void bitstack_push_bits(bitstack_t* bs, bit_code_t bits){
    while(bits.len > 0){
      int bit = bit_code_pop_msb(&bits);
      
      if(bit)
        bs->bits = bit_code_add_one(bs->bits);
      else
        bs->bits = bit_code_add_zero(bs->bits);
      if(bs->bits.len == 8){
        // bit_code_print(to_write);
        // printf(" ");
        cvector_push_back(bs->bytes, bs->bits.code);
        bs->bits = bit_code_empty();
      }
    }
}

void bitstack_push_bitstack(bitstack_t* bs, bitstack_t* bs2){

}