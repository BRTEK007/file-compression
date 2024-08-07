#include <gtest/gtest.h>

#include "byte_slice.hpp"
#include "byte_freq.hpp"
#include "tree.hpp"

using namespace oddcod;

TEST(ByteSlice, WriteReadEqual)
{
    //---ARRANGE
    ByteSlice byteSlice;

    //---ACT

    // 1101
    byteSlice.writeBit(true);
    byteSlice.writeBit(true);
    byteSlice.writeBit(false);
    byteSlice.writeBit(true);

    //---ASSERT

    // 1101 = D
    EXPECT_EQ(byteSlice.getByte(), (unsigned char)'\x0D');

    // 1101
    EXPECT_EQ(byteSlice.readBit(), true);
    EXPECT_EQ(byteSlice.readBit(), true);
    EXPECT_EQ(byteSlice.readBit(), false);
    EXPECT_EQ(byteSlice.readBit(), true);
}

TEST(BitCode, WriteReadEqual)
{
    //---ARRANGE
    BitCode code;

    //---ACT
    code.writeBit(true);
    code.writeBit(true);
    code.writeBit(false);
    code.writeBit(true);

    //---ASSERT
    EXPECT_EQ(code.readBit(), true);
    EXPECT_EQ(code.readBit(), true);
    EXPECT_EQ(code.readBit(), false);
    EXPECT_EQ(code.readBit(), true);
}

TEST(Tree, CreateFromAllBytesValid)
{
    //---ARRANGE
    std::vector<ByteFreq> byteFreqArr;
    for (int i = 0; i < 256; i++)
    {
        ByteFreq bf;
        bf.byte = i;
        bf.freq = 1;
        byteFreqArr.push_back(bf);
    }
    auto tree = Tree(byteFreqArr);

    //---ACT
    std::array<BitCode, 256> codes;
    tree.extractCodes(codes);

    //---ASSERT
    for (int i = 0; i < 256; i++)
    {
        assert(codes[i].size() > 0);
    }
}

TEST(BitWriter, WriteCorrectBitsBytes)
{
    //---ARRANGE
    std::ostringstream stream;
    auto bitOstream = StreamBitWriter(stream);
    //---ACT
    // 1001 11011000(D8) 0110
    bitOstream.write(true);
    bitOstream.write(false);
    bitOstream.write(false);
    bitOstream.write(true);

    bitOstream.write((unsigned char)'\xD8');

    bitOstream.write(false);
    bitOstream.write(true);
    bitOstream.write(true);
    bitOstream.write(false);
    //---ASSERT
    // 10011101(9D) 10000110(86)
    auto bytes = stream.str().c_str();
    EXPECT_EQ((unsigned char)bytes[0], (unsigned char)'\x9D');
    EXPECT_EQ((unsigned char)bytes[1], (unsigned char)'\x86');
}

TEST(BitReader, ReadCorrectBitsBytes)
{
    //---ARRANGE
    std::istringstream stream("\x9D\x86"); // 10011101(9D) 10000110(86)
    auto bitIstream = StreamBitReader(stream);
    //---ACT & ASSERT
    EXPECT_FALSE(bitIstream.eof());
    // 1001 11011000(D8) 0110
    EXPECT_EQ(bitIstream.readBit(), true);
    EXPECT_EQ(bitIstream.readBit(), false);
    EXPECT_EQ(bitIstream.readBit(), false);
    EXPECT_EQ(bitIstream.readBit(), true);

    EXPECT_EQ(bitIstream.readByte(), (unsigned char)'\xD8');

    EXPECT_EQ(bitIstream.readBit(), false);
    EXPECT_EQ(bitIstream.readBit(), true);
    EXPECT_EQ(bitIstream.readBit(), true);
    EXPECT_EQ(bitIstream.readBit(), false);

    // std::cout << "<" << bitIstream.readBit() << ">";
    EXPECT_TRUE(bitIstream.eof());
}

TEST(BitReader, ReadAligned)
{
    std::istringstream stream("\x9D\x86"); // 10011101(9D) 10000110(86)
    auto bitIstream = StreamAlignedReader(stream);

    EXPECT_FALSE(bitIstream.eof());
    EXPECT_EQ(bitIstream.readWord(), 0x9D);
    EXPECT_FALSE(bitIstream.eof());
    EXPECT_EQ(bitIstream.readWord(), 0x86);
    EXPECT_TRUE(bitIstream.eof());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
