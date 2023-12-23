#include "decompress.hpp"
#include "bit_set.hpp"
#include "tree.hpp"
#include <stdint.h>
#include <stdio.h>
#include <iostream>

void decompress(const std::vector<unsigned char>& in_buffer, std::ostream& outStream){
  //create bit_set from in_buffer
  BitSet bit_set;
  bit_set.create_from_bytes(in_buffer);

  bit_set.begin_read();
  //read 4 bytes -> total_characters count
  unsigned char bytes[4];
  bytes[0] = bit_set.read_byte();
  bytes[1] = bit_set.read_byte();
  bytes[2] = bit_set.read_byte();
  bytes[3] = bit_set.read_byte();
  uint32_t total_byte_count = *(reinterpret_cast<uint32_t*>(bytes));
  //read 1 byte -> unique bytes count
  bytes[0] = bit_set.read_byte();
  bytes[1] = bit_set.read_byte();
  uint16_t unique_byte_count = *(reinterpret_cast<uint16_t*>(bytes)); 
  //
  printf("-------------------------\n");
  printf("DECOMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  //read huffman tree data
  Tree tree;
  tree.create_from_bitset(&bit_set, unique_byte_count);//TODO bug here
  std::vector<unsigned char> leaf_bytes;
  tree.extract_leaf_bytes(leaf_bytes);//TODO or here

  std::array<BitCode, 256> codes; 
  
  tree.extract_codes(codes);
 
  printf("BYTE   | CODE\n");
  printf("-----------\n");
  for(int i = 0; i < leaf_bytes.size(); i++){
    printf("%d (%c) | ", leaf_bytes[i], leaf_bytes[i]);
    std::cout<<codes[leaf_bytes[i]].to_string();
    printf("\n");
  }
  printf("-------------------------\n");
  

  uint32_t read_bytes = 0;
  tree.ptr_reset();
  while(read_bytes < total_byte_count){
    bool bit = bit_set.read_bit();
    
    if(bit)
      tree.ptr_right();
    else
      tree.ptr_left();
    
    if(tree.ptr_is_leaf()){
      outStream << tree.ptr_read_byte();
      tree.ptr_reset();
      read_bytes++;
    }
  }
}
