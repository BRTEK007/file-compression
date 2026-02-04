#ifndef BIT_ISTREAM_HPP
#define BIT_ISTREAM_HPP

#include <istream>
#include <vector>
#include <iostream>
#include <memory>
#include "byte_slice.hpp"
#include "oddcod.hpp"

namespace oddcod
{
    // Abstract class for reading by bits.
    class BitReader
    {
    public:
        BitReader() : m_byteSlice() {};
        bool readBit()
        {
            if (m_byteSlice.size() == 0)
            {
                readFullWord();
            }

            bool bit = m_byteSlice.readBit();

            return bit;
        }
        unsigned char readByte()
        {
            // perform 8 bit reads
            // TODO optimize this
            ByteSlice slice;
            while (!slice.full()) // TODO this may overlap with readBit, need to reset byteSlice
            {
                slice.writeBit(readBit());
            }
            return slice.getByte();
        }
        bool eof()
        {
            return m_byteSlice.size() == 0;
        }

        virtual void resetToBegin() = 0;

    protected:
        virtual void readFullWord() = 0;
        ByteSlice m_byteSlice;
    };

    // Abstract class for reading by bytes.
    class AlignedReader
    {
    public:
        virtual bool eof() = 0;
        virtual word_t readWord() = 0;
        virtual void resetToBegin() = 0;
        virtual size_t getSize() = 0;
    };

    // Reads by bits from a stream.
    class StreamBitReader : public BitReader
    {
    public:
        StreamBitReader(std::istream &stream) : BitReader(), m_stream(stream)
        {
            // prepera first byte slice
            readFullWord(); // messes up aligned read
        };
        // TODO check if read correct amout, check m_stream.fail()

        void resetToBegin() override
        {
            m_byteSlice.reset();
            m_stream.clear();
            m_stream.seekg(0, std::ios_base::beg);
        };

    protected:
        void readFullWord() override
        {
            unsigned char byte;
            m_stream.read(reinterpret_cast<char *>(&byte), 1);
            m_byteSlice.setByte(byte);
        };
        std::istream &m_stream;
    };

    // Reads by bytes from a stream.
    class StreamAlignedReader : public AlignedReader
    {
    public:
        StreamAlignedReader(std::istream &stream) : m_stream(stream)
        {
            updateCache();
        };

        bool eof() override
        {
            return m_stream.eof();
        };

        word_t readWord() override
        {
            auto res = cacheWord;
            updateCache();
            return res;
        };
        void resetToBegin() override
        {
            m_stream.clear();
            m_stream.seekg(0, std::ios_base::beg);
            updateCache();
        };
        size_t getSize() override
        {
            m_stream.seekg(0, std::ios::end);
            auto streamSize = m_stream.tellg();
            m_stream.seekg(0, std::ios::beg);
            return streamSize;
        };
    protected:
        void updateCache()
        {
            m_stream.read(reinterpret_cast<char *>(&cacheWord), 1);
        }

        word_t cacheWord;
        std::istream &m_stream;
    };

    // Creates bit or aligned readers on stream.
    class StreamReaderProvider
    {
    public:
        StreamReaderProvider(std::istream *stream) : m_stream(stream), m_used(false) {};

        std::shared_ptr<BitReader> createBitReader()
        {
            if (m_used)
            {
                return nullptr;
            }
            m_used = true;
            return std::make_shared<StreamBitReader>(*m_stream);
        };

        std::shared_ptr<AlignedReader> createAlignedReader()
        {
            if (m_used)
            {
                return nullptr;
            }
            m_used = true;
            return std::make_shared<StreamAlignedReader>(*m_stream);
        };

    private:
        std::istream *m_stream;
        bool m_used;
    };
};

#endif