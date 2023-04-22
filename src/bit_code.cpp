#include "bit_code.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

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
    bool bit = bits.test(0);
    bits = bits >> 1;
    len--;
    return bit;
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