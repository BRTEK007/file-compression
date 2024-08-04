#ifndef BIT_ISTREAM_HPP
#define BIT_ISTREAM_HPP

#include <istream>
#include <vector>
#include <iostream>
#include "byte_slice.hpp"
#include "oddcod.hpp"

namespace oddcod
{
    class Input
    {
    public:
        Input() : byteSlice() {};
        bool readBit()
        {
            if (byteSlice.size() == 0)
            {
                readFullWord();
            }

            bool bit = byteSlice.readBit();

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
            return byteSlice.size() == 0;
        }
        virtual bool eofAligend() = 0;

        virtual word_t readAligned() = 0;

        virtual void resetToBegin() = 0;

    protected:
        virtual void readFullWord() = 0;
        ByteSlice byteSlice;
    };

    class StreamInput : public Input
    {
    public:
        StreamInput(std::istream &stream) : Input(), m_stream(stream)
        {
            // prepera first byte slice
            readFullWord(); // messes up aligned read
        };

        // void writeToVec(std::vector<word_t> *outVec) override
        //{
        // m_stream.seekg(0, std::ios::end);
        // auto streamSize = m_stream.tellg();
        // m_stream.seekg(0, std::ios::beg);

        // outVec->clear();
        // outVec->resize(streamSize);
        // m_stream.read(reinterpret_cast<char *>(outVec->data()), streamSize);
        //};

        // TODO check if read correct amout, check m_stream.fail()

        bool eofAligend() override
        {
            return m_stream.eof();
        }

        word_t readAligned() override
        {
            if (byteSlice.full())
            {
                return readByte();
            }
            word_t word;
            m_stream.read(reinterpret_cast<char *>(&word), 1);
            return word;
        };

        void resetToBegin() override
        {
            m_stream.clear();
            m_stream.seekg(0, std::ios_base::beg);
        };

    protected:
        void readFullWord() override
        {
            unsigned char byte;
            m_stream.read(reinterpret_cast<char *>(&byte), 1);
            byteSlice.setByte(byte);
        };
        std::istream &m_stream;
    };
};

#endif