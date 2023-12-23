#include "bitostream.hpp"

BitOstream::BitOstream(std::ostream &s) : stream(s), byteSlice(){};

void BitOstream::write(bool bit)
{
    // TODO better ByteSlice
    byteSlice.write_bit(bit);
    if (byteSlice.len == BYTE_SLICE_BIT_COUNT)
    {
        stream << byteSlice.bits;
        byteSlice = ByteSlice();
    }
}

void BitOstream::flush()
{
    unsigned char byte = byteSlice.bits;
    byte = byte << (BYTE_SLICE_BIT_COUNT - byteSlice.len);
    stream << byte;
}

void BitOstream::write(unsigned char byte)
{
    ByteSlice slice;
    slice.set_byte(byte);
    write(slice);
}

void BitOstream::write(ByteSlice slice)
{
    // write slice bit by bit
    while (slice.len > 0)
    {
        write(slice.read_bit());
    }
}

void BitOstream::write(BitCode bc)
{
    while (bc.size() > 0)
    {
        write(bc.readBit());
    }
}