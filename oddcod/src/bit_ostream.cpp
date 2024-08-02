#include "bit_ostream.hpp"
BitOstream::BitOstream(ByteWritable &s) : m_byteWritable(s), m_byteSlice() {};

void BitOstream::write(bool bit)
{
    m_byteSlice.writeBit(bit);
    if (m_byteSlice.full())
    {
        m_byteWritable << m_byteSlice.getByte();
        m_byteSlice.reset();
    }
}

void BitOstream::flush()
{
    unsigned char byte = m_byteSlice.getByte();
    byte = byte << (BYTE_SLICE_BIT_COUNT - m_byteSlice.size());
    m_byteWritable << byte;
}

void BitOstream::write(unsigned char byte)
{
    ByteSlice slice;
    slice.setByte(byte);
    write(slice);
}

void BitOstream::write(ByteSlice slice)
{
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