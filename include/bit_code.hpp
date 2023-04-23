#ifndef BIT_CODE_H
#define BIT_CODE_H

#include <bitset>

//class for holding bit codes for bytes

template<std::size_t N>
class BitCode{
    private:
        int len;
        std::bitset<N> bits;
    public:
        BitCode();
        void writeBit(bool bit);
        bool readBit();
        int size();
        std::string to_string();
};

#endif