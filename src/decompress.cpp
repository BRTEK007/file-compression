#include "decompress.hpp"
#include "bit_set.hpp"
#include "tree.hpp"
#include <stdint.h>
#include <stdio.h>

std::vector<unsigned char> decompress(std::vector<unsigned char> in_buffer){
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
  node_t* root = tree_read_from_bitset(&bit_set, unique_byte_count);
  std::vector<unsigned char> leaf_bytes;
  
  tree_extract_leaf_bytes(root, &leaf_bytes);

  byte_slice_t* codes = (byte_slice_t*) calloc(256, sizeof(byte_slice_t)); 
  
  tree_extract_codes(root, codes);

  printf("BYTE   | CODE\n");
  printf("-----------\n");
  for(int i = 0; i < leaf_bytes.size(); i++){
    printf("%d (%c) | ", leaf_bytes[i], leaf_bytes[i]);
    byte_slice_print(codes[leaf_bytes[i]]);
    printf("\n");
  }
  printf("-------------------------\n");
  
  free(codes);

  //read compressed data
  std::vector<unsigned char> out_buffer;
  uint32_t read_bytes = 0;
  node_t* node = root;
  while(read_bytes < total_byte_count){
    bool bit = bit_set_read_bit(&bit_set);
    if(bit){
      node = node->right;
    }else{
      node = node->left;
    }
    if(node->leaf){
      out_buffer.push_back(node->byte);
      node = root;
      read_bytes++;
    }
  }

  bit_set_free(&bit_set);
  tree_free(root);
  return out_buffer;
}
