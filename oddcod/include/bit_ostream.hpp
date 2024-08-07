#ifndef BITOSTREAM_HPP
#define BITOSTREAM_HPP

#include <ostream>
#include <vector>
#include "byte_slice.hpp"
#include "bit_code.hpp"
#include "oddcod.hpp"

namespace oddcod
{

    class Writer
    {
    public:
        Writer() : m_byteSlice() {};
        void write(bool bit)
        {
            m_byteSlice.writeBit(bit);
            if (m_byteSlice.full())
            {
                writeFullWord(m_byteSlice.getByte());
                m_byteSlice.reset();
            }
        }
        void write(ByteSlice slice)
        {
            // TODO optimize this
            while (slice.size() > 0)
            {
                write(slice.readBit());
            }
        }
        void write(BitCode bc)
        {
            // TODO optimize this
            while (bc.size() > 0)
            {
                write(bc.readBit());
            }
        }
        void write(word_t byte)
        {
            ByteSlice slice;
            slice.setByte(byte);
            write(slice);
        }
        void flush()
        {
            word_t byte = m_byteSlice.getByte();
            byte = byte << (BYTE_SLICE_BIT_COUNT - m_byteSlice.size());
            writeFullWord(byte);
        }
        virtual void writeAligned(const word_t *data, size_t dataSize) = 0;

    protected:
        virtual void writeFullWord(word_t word) = 0;
        ByteSlice m_byteSlice;
    };

    class StreamWriter : public Writer
    {
    public:
        StreamWriter(std::ostream &stream) : Writer(), m_stream(stream) {};

        void writeAligned(const word_t *data, size_t dataSize) override
        {
            m_stream.write(reinterpret_cast<const char *>(data), dataSize);
        }

    protected:
        void writeFullWord(word_t word) override
        {
            m_stream << word;
        }
        std::ostream &m_stream;
    };
};

#endif