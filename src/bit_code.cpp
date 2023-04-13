#include "bit_code.hpp"
// #include "c-vector/cvector.h"

void bit_code_init(bit_code_t* bc){
    // cvector_vector_type(bool) bits = NULL;
    // cvector_reserve(bits, 256);
    // bc->bits = bits;
    // bc->bits
}

void bit_code_add_bit(bit_code_t* bc, bool bit){
    // cvector_push_back(bc->bits, bit);
    bc->bits.push_back(bit);
}

void bit_code_free(bit_code_t* bc){
//    cvector_free(bc->bits); 
}

// BitCode::BitCode(){
//     this->bits.reserve(256);
// }

// void BitCode::addBit(bool bit){
//     this->bits.push_back(bit);
// }

// BitCode::~BitCode(){}

// class BitCode{
//     private:
//     std::vector<bool> bits;
//     ~BitCode();
//     public:
//         BitCode();
//         void addBit(bool bit);
// };