#include "decompress.hpp"
#include "tree.hpp"
#include "bitistream.hpp"
#include <stdint.h>
#include <stdio.h>
#include <iostream>

void decompress(std::istream &inStream, std::ostream &outStream)
{
  // create BitIstream from in_buffer
  auto bitIstream = BitIstream(inStream);

  // read 4 bytes -> total_characters count
  unsigned char bytes[4];
  bytes[0] = bitIstream.readByte();
  bytes[1] = bitIstream.readByte();
  bytes[2] = bitIstream.readByte();
  bytes[3] = bitIstream.readByte();
  uint32_t total_byte_count = *(reinterpret_cast<uint32_t *>(bytes));
  // read 1 byte -> unique bytes count
  bytes[0] = bitIstream.readByte();
  bytes[1] = bitIstream.readByte();
  uint16_t unique_byte_count = *(reinterpret_cast<uint16_t *>(bytes));
  //
  printf("-------------------------\n");
  printf("DECOMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  // read huffman tree data
  Tree tree;
  tree.readFrom(bitIstream, unique_byte_count); // TODO bug here
  std::vector<unsigned char> leaf_bytes;
  tree.extractLeafBytes(leaf_bytes); // TODO or here

  std::array<BitCode, 256> codes;

  tree.extractCodes(codes);

  printf("BYTE   | CODE\n");
  printf("-----------\n");
  for (size_t i = 0; i < leaf_bytes.size(); i++)
  {
    printf("%d (%c) | ", leaf_bytes[i], leaf_bytes[i]);
    std::cout << codes[leaf_bytes[i]].to_string();
    printf("\n");
  }
  printf("-------------------------\n");

  uint32_t read_bytes = 0;
  tree.ptrReset();
  while (read_bytes < total_byte_count)
  {
    bool bit = bitIstream.readBit();

    if (bit)
      tree.ptrRight();
    else
      tree.ptrLeft();

    if (tree.ptrIsLeaf())
    {
      auto byte = tree.ptrReadByte();
      outStream.write(reinterpret_cast<const char *>(&byte), 1);
      tree.ptrReset();
      read_bytes++;
    }
  }
}
