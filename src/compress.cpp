#include "compress.hpp"
#include "byte_freq.hpp"
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include "tree.hpp"
#include "bit_set.hpp"
#include "bit_code.hpp"

std::vector<unsigned char> compress(const std::vector<unsigned char>& in_buffer){
  uint32_t total_byte_count = in_buffer.size();

  std::vector<byte_freq_t> bf_arr = get_byte_frequencies(in_buffer);

  uint16_t unique_byte_count = bf_arr.size();
  
  Tree tree;
  tree.create_from_bytefreq(bf_arr);

  std::array<BitCode, 256> codes; 

  tree.extract_codes(codes);

  printf("-------------------------\n");
  printf("COMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  printf("BYTE   | FREQUENCY | CODE\n");
  printf("-------------------------\n");
  for(int i = 0; i < unique_byte_count; i++){
    byte_freq_t bf = bf_arr[i];
    if(std::isprint(bf.byte))
      printf("%d (%c) | %9.1f | ", bf.byte, bf.byte, (float)(100*bf.freq) / total_byte_count);
    else
      printf("%d | %9.1f | ", bf.byte, (float)(100*bf.freq) / total_byte_count);
    std::cout<<codes[bf.byte].to_string();
    printf("\n");
  }
  printf("-------------------------\n");


  BitSet bit_set;

  //write 4 bytes -> total bytes count
  unsigned char* bytes = (unsigned char *)&total_byte_count;
  bit_set.write_byte(bytes[0]);
  bit_set.write_byte(bytes[1]);
  bit_set.write_byte(bytes[2]);
  bit_set.write_byte(bytes[3]);
  //write 2 bytes -> unique bytes count
  bytes = (unsigned char*)&unique_byte_count;
  bit_set.write_byte(bytes[0]);
  bit_set.write_byte(bytes[1]);
  //write huffman tree data
  tree.write_to_bitset(&bit_set);
  //write compressed data
  for(uint32_t i = 0; i < total_byte_count; i++){
    unsigned char byte = in_buffer[i];
    // byte_slice_t slice = codes[byte];
    BitCode code = codes[byte];
    bit_set.write_bitcode(code);
  }
  bit_set.end_write();

  std::vector<unsigned char> out_buffer = bit_set.extract_bytes();
  //extract to out_buffer from bit_set

  double reduction = 100.f * (1.f - ((double)out_buffer.size() / (double)in_buffer.size())); 
  printf("size after compression: %ld, %.1f%% reduction\n", out_buffer.size(), reduction);
  printf("-------------------------\n");
  
  return out_buffer;
}