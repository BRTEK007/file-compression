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

    template <typename T>
    class Input
    {
    public:
        virtual Input(T &source) = 0;

        virtual word_t read() = 0;
        virtual bool canRead() = 0;
        virtual void readAll(std::vector<word_t> *outVector) = 0;
    };

    template <>
    class Input<std::istream>
    {
    public:
        Input(std::istream &stream) : m_stream(stream) {}
        word_t read() {

        };
        bool canRead() {

        };
        void readAll(std::vector<word_t> *outVector)
        {
            assert(outVector);
            m_stream.seekg(0, std::ios::end);
            auto streamSize = m_stream.tellg();
            m_stream.seekg(0, std::ios::beg);

            outVector->clear();
            outVector->resize(streamSize);
            m_stream.read(reinterpret_cast<char *>(outVector->data()), streamSize);
        }
        std::istream &m_stream;
    };
    template <typename T>
    class Output
    {
    public:
        explicit Output(T &destination) : m_dest(destination)
        {
            init();
        }

        virtual void write(word_t word) = 0;

    private:
        virtual void init() = 0;
        T &m_dest;
    };

    template <>
    class Output<std::ostream &>
    {
    public:
        void write(word_t word)
        {
        }

    private:
        void init()
        {
        }
    };

    namespace huffman
    {
        template <typename TI, typename TO>
        Result encode(Input<TI> input, Output<TO> output);
        template <typename TI, typename TO>
        Result decode(Input<TI> input, Output<TO> output);
    };
};

#endif
