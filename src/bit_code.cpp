#include "bit_code.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

template class BitCode<256>;
template class BitCode<8>;

template<std::size_t N>
BitCode<N>::BitCode(){
    bits.reset();
    len = 0;
}

template<std::size_t N>
void BitCode<N>::writeBit(bool bit){
    bits.set(len, bit);
    len++;
}

template<std::size_t N>
bool BitCode<N>::readBit(){
    bool bit = bits.test(0);
    bits = bits >> 1;
    len--;
    return bit;
}

template<std::size_t N>
int BitCode<N>::size(){
    return len;
}

template<std::size_t N>
std::string BitCode<N>::to_string(){
    auto s = bits.to_string();
    s.erase(0, N - len);
    std::reverse(s.begin(), s.end());
    return s;
}