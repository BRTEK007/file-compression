#ifndef BIT_CODE_H
#define BIT_CODE_H

#include <bitset>

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

#endif