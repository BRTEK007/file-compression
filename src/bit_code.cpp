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

void BitCode::writeByte(unsigned char byte){
    for(int i = 0; i < 8; i++){
        int bit = byte & 1u >> i;
        bits.set(len, static_cast<bool>(bit));
        len++;
    }
}

bool BitCode::readBit(){
    bool bit = bits.test(0);
    bits = bits >> 1;
    len--;
    return bit;
}

unsigned char BitCode::readByte(){
    BitCode temp = *this;
    unsigned char byte = 0;
    for(int i = 0; i < 8; i++){
        unsigned bit = static_cast<unsigned>(bits.test(i));
        byte = byte | bit << (7-i);
    }
    bits = bits >> 8;
    len -= 8;
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