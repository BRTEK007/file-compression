#ifndef BIT_CODE_H
#define BIT_CODE_H

#include <bitset>

//class for holding bit codes for bytes

class BitCode{
    private:
        int len;
        std::bitset<256> bits;
    public:
        BitCode();
        void writeBit(bool bit);
        void writeByte(unsigned char byte);
        bool readBit();
        unsigned char readByte();
        int size();
        std::string to_string();
};

#endif