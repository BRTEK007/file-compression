#ifndef ODD_CODING_LIB_HPP
#define ODD_CODING_LIB_HPP

#include <istream>
#include <ostream>
#include <assert.h>

namespace oddcod
{

    using word_t = unsigned char;

    enum class Result
    {
        OK = 0,
        DECODE_ERR = 1,
        ENCODE_ERR = 2
    };

    class Input
    {
    public:
        virtual word_t read() = 0;
        virtual bool canRead() = 0;
        virtual void readAll(std::vector<word_t> *outVector) = 0;

    private:
        std::istream &stream;
        ByteSlice byteSlice;
    };

    //

    class BitIstream
    {
        std::istream &stream;
        ByteSlice byteSlice;

    public:
        BitIstream(std::istream &stream);
        bool readBit();
        unsigned char readByte();
        bool eof();
    };

    //

    class Output
    {
    public:
        virtual void write(word_t word) = 0;
    };

    class StreamInput : public Input
    {
    public:
        StreamInput(std::istream &stream) : m_stream(stream) {}

        word_t read() override
        {
        }
        bool canRead() override
        {
        }
        void readAll(std::vector<word_t> *outVector)
        {
            m_stream.seekg(0, std::ios::end);
            auto streamSize = m_stream.tellg();
            m_stream.seekg(0, std::ios::beg);

            outVector->clear();
            outVector->resize(streamSize);
            m_stream.read(reinterpret_cast<char *>(outVector->data()), streamSize);
        }

    private:
        std::istream &m_stream;
    };

    class StreamOutput : public Output
    {
    public:
        StreamOutput(std::istream &stream) : m_stream(stream) {}

        void write(word_t word) override
        {
        }

    private:
        std::istream &m_stream;
    };

    namespace huffman
    {
        Result encode(Input &input, Output &output);
        Result decode(Input &input, Output &output);
    };
};

#endif
