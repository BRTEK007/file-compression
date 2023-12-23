#include "bitistream.hpp"

BitIstream::BitIstream(const std::vector<unsigned char> &buffer) : buffer(buffer), byteSlice(), index(0)
{
    byteSlice.set_byte(buffer[index]);
    index++;
};

bool BitIstream::readBit()
{
    bool bit = byteSlice.read_bit();

    if (byteSlice.len == 0)
    { // slice is empty -> copy next byte to slice
        byteSlice.set_byte(buffer[index]);
        index++;
    }

    return bit;
}

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