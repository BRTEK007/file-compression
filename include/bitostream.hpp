#ifndef BITOSTREAM_HPP
#define BITOSTREAM_HPP

#include <ostream>
#include "byte_slice.hpp"
#include "bit_code.hpp"

class BitOstream{
        std::ostream& stream;
        ByteSlice byteSlice;
    public:
        BitOstream(std::ostream& s);
        void writeBit(bool bit);
        void writeByte(unsigned char byte);
        void writeSlice(ByteSlice slice);
        void writeBitcode(BitCode bc);//TODO use function overloading write(X)...
        void flush();
};

#endif