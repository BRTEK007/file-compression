#include "oddcod.hpp"

#include "tree.hpp"
#include "bit_istream.hpp"

oddcod::CodingResult oddcod::huffman::decode(std::istream &inStream, std::ostream &outStream)
{
  auto bitIstream = BitIstream(inStream);

  // read 2 bytes -> unique bytes count
  unsigned char bytes[2];
  bytes[0] = bitIstream.readByte(); // TODO possible error/ might file be too small
  bytes[1] = bitIstream.readByte();
  uint16_t uniqueByteCount = *(reinterpret_cast<uint16_t *>(bytes));

  Tree tree;
  tree.readFrom(bitIstream, uniqueByteCount); // TODO possible error

  std::vector<unsigned char> leafBytes;
  tree.extractLeafBytes(leafBytes);

  std::array<BitCode, 256> codes;
  tree.extractCodes(codes);

  tree.ptrReset();
  while (!bitIstream.eof())
  {
    bool bit = bitIstream.readBit();

    if (bit)
      tree.ptrRight(); // TODO possible null refrence
    else
      tree.ptrLeft(); // TODO possible null refrence

    if (tree.ptrIsLeaf())
    {
      auto byte = tree.ptrReadByte();
      outStream.write(reinterpret_cast<const char *>(&byte), 1);
      tree.ptrReset();
    }
  }
  return oddcod::CodingResult::OK;
}
