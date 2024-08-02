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

    union IOType
    {
        std::istream *inStream;
        std::ostream *outStream;
        const std::vector<word_t> *inVec;
        std::vector<word_t> *outVec;

        IOType() : inStream(nullptr) {}
    };

    enum class IOTypeFlags
    {
        IN_STREAM = 1,
        OUT_STREAM,
        IN_VEC,
        OUT_VEC
    };

    class Input
    {
    public:
        Input(std::istream &inStream) : m_srcFlags(IOTypeFlags::IN_STREAM)
        {
            m_src.inStream = &inStream;
        };
        Input(const std::vector<word_t> &inVec);

    private:
        IOType m_src;
        IOTypeFlags m_srcFlags;
    };

    class Output
    {
    };

    namespace huffman
    {
        Result encode(std::istream &inStream, std::ostream &outStream);
        Result decode(std::istream &inStream, std::ostream &outStream);
    };
};

#endif
