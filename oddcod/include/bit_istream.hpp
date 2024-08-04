#ifndef BIT_ISTREAM_HPP
#define BIT_ISTREAM_HPP

#include <istream>
#include <vector>
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
            ByteSlice slice;
            while (!slice.full()) // TODO this may overlap with readBit, need to reset byteSlice
            {
                slice.writeBit(readBit());
            }
            return slice.getByte();
        }
        bool eof()
        {
            return byteSlice.size() > 0;
        }

        virtual void writeToVec(std::vector<word_t> *outVec) = 0;

    protected:
        virtual void readFullWord() = 0;
        ByteSlice byteSlice;
    };

    class StreamInput : public Input
    {
    public:
        StreamInput(std::istream &stream) : Input(), m_stream(stream) {};

        void writeToVec(std::vector<word_t> *outVec) override
        {
            m_stream.seekg(0, std::ios::end);
            auto streamSize = m_stream.tellg();
            m_stream.seekg(0, std::ios::beg);

            outVec->clear();
            outVec->resize(streamSize);
            m_stream.read(reinterpret_cast<char *>(outVec->data()), streamSize);
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