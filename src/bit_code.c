#include "bit_code.h"
#include "c-vector/cvector.h"

void bit_code_init(bit_code_t* bc){
    cvector_vector_type(bool) bits = NULL;
    cvector_reserve(bits, 256);
    bc->bits = bits;
}

void bit_code_add_bit(bit_code_t* bc, bool bit){
    cvector_push_back(bc->bits, bit);
}

void bit_code_free(bit_code_t* bc){
   cvector_free(bc->bits); 
}