#include "decompress.hpp"

#include "tree.hpp"
#include "bit_istream.hpp"

#include <iostream>

void decompress(std::istream &inStream, std::ostream &outStream)
{
  // create BitIstream from in_buffer
  auto bitIstream = BitIstream(inStream);

  // read 4 bytes -> total_characters count
  unsigned char bytes[2];
  //  read 1 byte -> unique bytes count
  bytes[0] = bitIstream.readByte(); // TODO possible error/ might file be too small
  bytes[1] = bitIstream.readByte();
  uint16_t uniqueByteCount = *(reinterpret_cast<uint16_t *>(bytes));
  //
  printf("-------------------------\n");
  printf("DECOMPRESSING %d UNIQUE\n", uniqueByteCount);
  printf("-------------------------\n");
  // read huffman tree data
  Tree tree;
  tree.readFrom(bitIstream, uniqueByteCount); // TODO possible error
  std::vector<unsigned char> leafBytes;
  tree.extractLeafBytes(leafBytes);

  std::array<BitCode, 256> codes;

  tree.extractCodes(codes);

  printf("BYTE   | CODE\n");
  printf("-----------\n");
  for (size_t i = 0; i < leafBytes.size(); i++)
  {
    printf("%d (%c) | ", leafBytes[i], leafBytes[i]);
    std::cout << codes[leafBytes[i]].to_string();
    printf("\n");
  }
  printf("-------------------------\n");

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
}
