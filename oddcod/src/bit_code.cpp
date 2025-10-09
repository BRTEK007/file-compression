#include "bit_code.hpp"
#include <algorithm>
namespace oddcod
{
    BitCode::BitCode() : len(0)
    {
        bits.reset();
    }

    void BitCode::writeBit(bool bit)
    {
        bits.set(len, bit);
        len++;
    }

    bool BitCode::readBit()
    {
        bool bit = bits.test(0);
        bits = bits >> 1;
        len--;
        return bit;
    }

    size_t BitCode::size()
    {
        return len;
    }

    std::string BitCode::to_string()
    {
        auto s = bits.to_string();
        s.erase(0, 256 - len);
        std::reverse(s.begin(), s.end());
        return s;
    }
};