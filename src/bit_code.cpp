#include "bit_code.hpp"

void bit_code_init(bit_code_t* bc){
    bc->bits.reserve(256);
}

void bit_code_add_bit(bit_code_t* bc, bool bit){
    bc->bits.push_back(bit);
}

void bit_code_free(bit_code_t* bc){
}