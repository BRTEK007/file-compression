#include "bitostream.hpp"

BitOstream::BitOstream(std::ostream &s) : stream(s), byteSlice(){};

void BitOstream::write(bool bit)
{
    // TODO better ByteSlice
    byteSlice.writeBit(bit);
    if (byteSlice.full())
    {
        stream << byteSlice.getByte();
        byteSlice = ByteSlice();
    }
}

void BitOstream::flush()
{
    unsigned char byte = byteSlice.getByte();
    byte = byte << (BYTE_SLICE_BIT_COUNT - byteSlice.size());
    stream << byte;
}

void BitOstream::write(unsigned char byte)
{
    ByteSlice slice;
    slice.setByte(byte);
    write(slice);
}

void BitOstream::write(ByteSlice slice)
{
    // write slice bit by bit
    while (slice.size() > 0)
    {
        write(slice.readBit());
    }
}

void BitOstream::write(BitCode bc)
{
    while (bc.size() > 0)
    {
        write(bc.readBit());
    }
}