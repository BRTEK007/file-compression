#ifndef BITOSTREAM_HPP
#define BITOSTREAM_HPP

#include <ostream>
#include <vector>
#include <memory>
#include "byte_slice.hpp"
#include "bit_code.hpp"
#include "oddcod.hpp"

namespace oddcod
{
    // Abstract class for writing by bits.
    class BitWriter
    {
    public:
        BitWriter() : m_byteSlice() {};
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

    protected:
        virtual void writeFullWord(word_t word) = 0;
        ByteSlice m_byteSlice;
    };

    // Abstract class for writing by bytes. 
    class AlignedWriter
    {
    public:
        virtual void write(word_t *data, size_t dataSize) = 0;
    };

    // Writes by bits to a stream.
    class StreamBitWriter : public BitWriter
    {
    public:
        StreamBitWriter(std::ostream &stream) : BitWriter(), m_stream(stream) {};

    protected:
        void writeFullWord(word_t word) override
        {
            m_stream << word;
        }
        std::ostream &m_stream;
    };

    // Writes by bytes to a stream.
    class StreamAlignedWriter : public AlignedWriter
    {
    public:
        StreamAlignedWriter(std::ostream &stream) : m_stream(stream) {};
        void write(word_t *data, size_t dataSize)
        {
            m_stream.write(reinterpret_cast<const char *>(data), dataSize);
        }

    protected:
        std::ostream &m_stream;
    };
    
    // Creates bit or aligned writers on stream.
    class StreamWriterProvider
    {
    public:
        StreamWriterProvider(std::ostream *stream) : m_stream(stream), m_used(false) {};

        std::shared_ptr<BitWriter> createBitWriter()
        {
            if (m_used)
            {
                return nullptr;
            }
            m_used = true;
            return std::make_shared<StreamBitWriter>(*m_stream);
        };

        std::shared_ptr<AlignedWriter> createAlignedWriter()
        {
            if (m_used)
            {
                return nullptr;
            }
            m_used = true;
            return std::make_shared<StreamAlignedWriter>(*m_stream);
        };

    private:
        std::ostream *m_stream;
        bool m_used;
    };
};

#endif