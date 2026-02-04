#ifndef BIT_CODE_H
#define BIT_CODE_H

#include <bitset>

namespace oddcod
{
    // Stores 256 bit long code, with read and write by individual bits.
    class BitCode
    {
    private:
        size_t len;
        std::bitset<256> bits;

    public:
        BitCode();
        void writeBit(bool bit);
        bool readBit();
        size_t size();
        std::string to_string();
    };
};

#endif