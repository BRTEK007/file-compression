#include "compress.hpp"
#include "byte_freq.hpp"
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include "tree.hpp"
#include "bit_ostream.hpp"
#include "bit_code.hpp"
#include "bit_ostream.hpp"

inline void streamToVector(std::istream &inStream, std::vector<unsigned char> &outVector)
{
  inStream.seekg(0, std::ios::end);
  auto streamSize = inStream.tellg();
  inStream.seekg(0, std::ios::beg);

  outVector.clear();
  outVector.resize(streamSize);
  inStream.read(reinterpret_cast<char *>(outVector.data()), streamSize);
}

void compress(std::istream &inStream, std::ostream &outStream)
{
  std::vector<unsigned char> inBuffer;

  streamToVector(inStream, inBuffer);
  // TODO inStream to inBuffer

  auto byteFreqArr = findByteFrequencies(inBuffer);

  uint16_t uniqueByteCount = byteFreqArr.size();

  auto tree = Tree(byteFreqArr);

  std::array<BitCode, 256> codes;

  tree.extractCodes(codes);

  auto totalByteCount = inBuffer.size();
  printf("-------------------------\n");
  printf("COMPRESSING %ld BYTES, %d UNIQUE\n", totalByteCount, uniqueByteCount);
  printf("-------------------------\n");
  printf("BYTE   | FREQUENCY | CODE\n");
  printf("-------------------------\n");
  for (int i = 0; i < uniqueByteCount; i++)
  {
    ByteFreq bf = byteFreqArr[i];
    if (std::isprint(bf.byte))
      printf("%d (%c) | %9.1f | ", bf.byte, bf.byte, (float)(100 * bf.freq) / totalByteCount);
    else
      printf("%d | %9.1f | ", bf.byte, (float)(100 * bf.freq) / totalByteCount);
    std::cout << codes[bf.byte].to_string();
    printf("\n");
  }
  printf("-------------------------\n");

  auto bitOstream = BitOstream(outStream);

  //  write 2 bytes -> unique bytes count
  unsigned char *bytes = reinterpret_cast<unsigned char *>(&uniqueByteCount);
  bitOstream.write(bytes[0]);
  bitOstream.write(bytes[1]);
  // write huffman tree data
  tree.writeTo(bitOstream);
  // write compressed data
  for (auto byte : inBuffer)
  {
    BitCode code = codes[byte];
    bitOstream.write(code);
  }
  bitOstream.flush();

  // extract to out_buffer from bitOutSream

  // double reduction = 100.f * (1.f - ((double)out_buffer.size() / (double)inBuffer.size()));
  // printf("size after compression: %ld, %.1f%% reduction\n", out_buffer.size(), reduction);
  // printf("-------------------------\n");
}