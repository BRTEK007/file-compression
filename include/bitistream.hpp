#ifndef BIT_ISTREAM_HPP
#define BIT_ISTREAM_HPP

#include <vector>
#include "byte_slice.hpp"
#include <stdlib.h>

class BitIstream
{
    const std::vector<unsigned char> &buffer; // use istream
    ByteSlice byteSlice;
    size_t index;

public:
    BitIstream(const std::vector<unsigned char> &buffer);
    bool readBit();
    unsigned char readByte();
};

#endif