#ifndef ODD_CODING_LIB_HPP
#define ODD_CODING_LIB_HPP

#include <istream>
#include <ostream>

namespace oddcod
{
    enum class CodingResult
    {
        OK = 0,
        DECODE_ERR = 1,
        ENCODE_ERR = 2
    };

    namespace huffman
    {
        CodingResult decode(const char *inBuffer, char *outBuffer);
        CodingResult encode(const char *inBuffer, char *outBuffer);
        CodingResult decode(std::istream &inStream, std::ostream &outStream);
        CodingResult encode(std::istream &inStream, std::ostream &outStream);
    };
};

#endif
