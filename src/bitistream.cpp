#include "bitistream.hpp"

BitIstream::BitIstream(std::istream &stream) : stream(stream), byteSlice(){};

bool BitIstream::readBit()
{
    if (byteSlice.len == 0)
    {
        unsigned char byte;
        stream.read(reinterpret_cast<char *>(&byte), 1);
        byteSlice.set_byte(byte);
    }

    bool bit = byteSlice.read_bit();

    return bit;
}

// TODO implement eof

unsigned char BitIstream::readByte()
{
    // perform 8 bit reads
    ByteSlice slice;
    while (slice.len < BYTE_SLICE_BIT_COUNT)
    {
        slice.write_bit(readBit());
    }
    return slice.bits;
}