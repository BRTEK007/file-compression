#ifndef BITOSTREAM_HPP
#define BITOSTREAM_HPP

#include <ostream>
#include <vector>
#include "byte_slice.hpp"
#include "bit_code.hpp"

class ByteWritable
{
public:
    virtual ~ByteWritable() = default;

    virtual void write(char c) = 0;

    ByteWritable &operator<<(char c)
    {
        this->write(c);
        return *this;
    }
};
//
class StreamByteWritable : public ByteWritable
{
public:
    StreamByteWritable(std::ostream &os) : m_outStream(os) {}

    void write(char c) override
    {
        m_outStream << c;
    }

private:
    std::ostream &m_outStream;
};
//
class VectorByteWritable : public ByteWritable
{
public:
    VectorByteWritable(std::vector<char> &outBuffer) : m_outBuffer(outBuffer) {};

    void write(char c) override
    {
        m_outBuffer.push_back(c);
    }

private:
    std::vector<char> &m_outBuffer;
};
//
class BitOstream
{
public:
    BitOstream(ByteWritable &s);
    void write(bool bit);
    void write(unsigned char byte);
    void write(ByteSlice slice);
    void write(BitCode bc);
    void flush();

private:
    ByteWritable &m_byteWritable;
    ByteSlice m_byteSlice;
};

#endif