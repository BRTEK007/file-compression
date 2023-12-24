#include "bit_istream.hpp"

BitIstream::BitIstream(std::istream &stream) : stream(stream), byteSlice(){};

bool BitIstream::readBit()
{
    if (byteSlice.size() == 0)
    {
        unsigned char byte;
        stream.read(reinterpret_cast<char *>(&byte), 1);
        byteSlice.setByte(byte);
    }

    bool bit = byteSlice.readBit();

    return bit;
}

// TODO implement eof

bool BitIstream::eof()
{
    return byteSlice.size() > 0;
}

unsigned char BitIstream::readByte()
{
    // perform 8 bit reads
    ByteSlice slice;
    while (slice.size() < BYTE_SLICE_BIT_COUNT) // TODO this may overlap with readBit, need to reset byteSlice
    {
        slice.writeBit(readBit());
    }
    return slice.getByte();
}