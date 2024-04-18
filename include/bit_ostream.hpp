#ifndef BITOSTREAM_HPP
#define BITOSTREAM_HPP

#include <ostream>
#include "byte_slice.hpp"
#include "bit_code.hpp"

class BitOstream
{
    std::ostream &stream;
    ByteSlice byteSlice;

public:
    BitOstream(std::ostream &s);
    void write(bool bit);
    void write(unsigned char byte);
    void write(ByteSlice slice);
    void write(BitCode bc);
    void flush();
};

#endif