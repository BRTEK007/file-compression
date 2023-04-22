#include "bit_code.hpp"
#include <stdexcept>
#include <iostream>

BitCode::BitCode(){
    bits.reset();
    len = 0;
}

void BitCode::writeBit(bool bit){
    // if(len == 256) throw std::runtime_error("can't add bit to full bitcode");
    bits.set(len, bit);
    len++;
}

bool BitCode::readBit(){
    // if(len == 0) throw std::runtime_error("can't read from empty bitcode");
    bool bit = bits.test(len-1);
    len--;
    return bit;
}

int BitCode::size(){
    return len;
}

void BitCode::print(){
    std::cout<<bits.to_string();
}