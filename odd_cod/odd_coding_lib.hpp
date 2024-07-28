#ifndef ODD_CODING_LIB_HPP
#define ODD_CODING_LIB_HPP

namespace oddcl
{
    enum OperationResult : int
    {
        OK = 0,
        DECODE_ERR = 1,
        ENCODE_ERR = 2
    };

    namespace huffman
    {
        OperationResult decode(const char *inBuffer, char *outBuffer);
        OperationResult encode(const char *inBuffer, char *outBuffer);
    };
};

#endif
