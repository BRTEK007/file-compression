#include "decompress.hpp"
#include "bit_set.hpp"
#include "tree.hpp"
#include <stdint.h>
#include <stdio.h>
#include <iostream>

std::vector<unsigned char> decompress(const std::vector<unsigned char>& in_buffer){
  //create bit_set from in_buffer
  bit_set_t bit_set;
  bit_set_init_from_bytes(&bit_set, in_buffer);

  bit_set_begin_read(&bit_set);
  //read 4 bytes -> total_characters count
  unsigned char bytes[4];
  bytes[0] = bit_set_read_byte(&bit_set);
  bytes[1] = bit_set_read_byte(&bit_set);
  bytes[2] = bit_set_read_byte(&bit_set);
  bytes[3] = bit_set_read_byte(&bit_set);
  uint32_t total_byte_count = *((uint32_t*)bytes);
  //read 1 byte -> unique bytes count
  unsigned char unique_byte_count = bit_set_read_byte(&bit_set); 
  //
  printf("-------------------------\n");
  printf("DECOMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  //read huffman tree data
  Tree tree;
  tree.create_from_bitset(&bit_set, unique_byte_count);
  std::vector<unsigned char> leaf_bytes;
  
  tree.extract_leaf_bytes(leaf_bytes);

  std::array<BitCode<256>, 256> codes; 
  
  tree.extract_codes(codes);

  printf("BYTE   | CODE\n");
  printf("-----------\n");
  for(int i = 0; i < leaf_bytes.size(); i++){
    printf("%d (%c) | ", leaf_bytes[i], leaf_bytes[i]);
    std::cout<<codes[leaf_bytes[i]].to_string();
    printf("\n");
  }
  printf("-------------------------\n");
  

  std::vector<unsigned char> out_buffer;
  uint32_t read_bytes = 0;
  tree.ptr_reset();
  while(read_bytes < total_byte_count){
    bool bit = bit_set_read_bit(&bit_set);
    
    if(bit)
      tree.ptr_right();
    else
      tree.ptr_left();
    
    if(tree.ptr_is_leaf()){
      out_buffer.push_back(tree.ptr_read_byte());
      tree.ptr_reset();
      read_bytes++;
    }
  }

  bit_set_free(&bit_set);
  return out_buffer;
}
