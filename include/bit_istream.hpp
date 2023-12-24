#ifndef BIT_ISTREAM_HPP
#define BIT_ISTREAM_HPP

#include <istream>
#include "byte_slice.hpp"
#include <stdlib.h>

class BitIstream
{
    std::istream &stream;
    ByteSlice byteSlice;

public:
    BitIstream(std::istream &stream);
    bool readBit();
    unsigned char readByte();
    bool eof();
};

#endif