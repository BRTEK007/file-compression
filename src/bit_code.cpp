#include "bit_code.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>


BitCode::BitCode(){
    bits.reset();
    len = 0;
}

void BitCode::writeBit(bool bit){
    bits.set(len, bit);
    len++;
}

bool BitCode::readBit(){
    bool bit = bits.test(0);
    bits = bits >> 1;
    len--;
    return bit;
}

unsigned char BitCode::readByte(){//TODO implement
    unsigned char byte = 0;
    return byte;
}

int BitCode::size(){
    return len;
}

std::string BitCode::to_string(){
    auto s = bits.to_string();
    s.erase(0, 256 - len);
    std::reverse(s.begin(), s.end());
    return s;
}