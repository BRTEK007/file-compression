#ifndef BIT_CODE_H
#define BIT_CODE_H

#include <stdbool.h>
#include <bitset>

//class for holding bit codes for bytes

class BitCode{
    private:
        int len;
        std::bitset<256> bits;
    public:
        BitCode();
        void writeBit(bool bit);
        bool readBit();
        int size();
        std::string to_string();
};

#endif