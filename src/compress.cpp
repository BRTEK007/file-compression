#include "compress.hpp"
#include "byte_freq.hpp"
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include "tree.hpp"
#include "bitostream.hpp"
#include "bitcode.hpp"
#include "bitostream.hpp"

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
  std::vector<unsigned char> in_buffer;

  streamToVector(inStream, in_buffer);
  // TODO inStream to in_buffer

  uint32_t total_byte_count = in_buffer.size();

  std::vector<ByteFreq> bf_arr = findByteFrequencies(in_buffer);

  uint16_t unique_byte_count = bf_arr.size();

  auto tree = Tree(bf_arr);

  std::array<BitCode, 256> codes;

  tree.extractCodes(codes);

  printf("-------------------------\n");
  printf("COMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  printf("BYTE   | FREQUENCY | CODE\n");
  printf("-------------------------\n");
  for (int i = 0; i < unique_byte_count; i++)
  {
    ByteFreq bf = bf_arr[i];
    if (std::isprint(bf.byte))
      printf("%d (%c) | %9.1f | ", bf.byte, bf.byte, (float)(100 * bf.freq) / total_byte_count);
    else
      printf("%d | %9.1f | ", bf.byte, (float)(100 * bf.freq) / total_byte_count);
    std::cout << codes[bf.byte].to_string();
    printf("\n");
  }
  printf("-------------------------\n");

  auto bitOstream = BitOstream(outStream);

  // write 4 bytes -> total bytes count
  unsigned char *bytes = reinterpret_cast<unsigned char *>(&total_byte_count);
  bitOstream.write(bytes[0]);
  bitOstream.write(bytes[1]);
  bitOstream.write(bytes[2]);
  bitOstream.write(bytes[3]);
  // write 2 bytes -> unique bytes count
  bytes = reinterpret_cast<unsigned char *>(&unique_byte_count);
  bitOstream.write(bytes[0]);
  bitOstream.write(bytes[1]);
  // write huffman tree data
  tree.writeTo(bitOstream);
  // write compressed data
  for (uint32_t i = 0; i < total_byte_count; i++)
  {
    unsigned char byte = in_buffer[i];
    BitCode code = codes[byte];
    bitOstream.write(code);
  }
  bitOstream.flush();

  // extract to out_buffer from bitOutSream

  // double reduction = 100.f * (1.f - ((double)out_buffer.size() / (double)in_buffer.size()));
  // printf("size after compression: %ld, %.1f%% reduction\n", out_buffer.size(), reduction);
  // printf("-------------------------\n");
}