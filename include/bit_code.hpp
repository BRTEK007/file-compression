#ifndef BIT_CODE_H
#define BIT_CODE_H

#include <stdbool.h>
#include <vector>

#define BIT_ONE true
#define BIT_ZERO false

//struct for holding bit codes for bytes

typedef struct bit_code_t{
    std::vector<bool> bits;
}bit_code_t;

void bit_code_init(bit_code_t* bc);

void bit_code_add_bit(bit_code_t* bc, bool bit);

void bit_code_free(bit_code_t* bc);

#endif