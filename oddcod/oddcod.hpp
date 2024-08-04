#ifndef ODD_CODING_LIB_HPP
#define ODD_CODING_LIB_HPP

#include <istream>
#include <ostream>

namespace oddcod
{
    using word_t = unsigned char;

    enum class Result
    {
        OK = 0,
        ERR_ENCODE_EMPTY = 1,
    };

    // have interface for INput and Output, extend with classes, pass that to a function, unify with Bitistream and BitOstream

    namespace huffman
    {
        Result encode(std::istream &inStream, std::ostream &outStream);
        Result decode(std::istream &inStream, std::ostream &outStream);
    };
};

#endif
