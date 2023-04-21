#include "compress.hpp"
#include "byte_freq.hpp"
#include "bit_code.hpp"
#include <stdint.h>
#include <stdio.h>
// #include "c-vector/cvector.h"
#include "tree.hpp"
#include "bit_set.hpp"

std::vector<unsigned char> compress(std::vector<unsigned char> in_buffer){
  // uint32_t total_byte_count = cvector_size(in_buffer);
  uint32_t total_byte_count = in_buffer.size();

  std::vector<byte_freq_t> bf_arr = get_byte_freq_arr(in_buffer);

  unsigned char unique_byte_count = bf_arr.size();
  
  node_t* root = create_tree(bf_arr);
  
  byte_slice_t* codes = (byte_slice_t*)calloc(256, sizeof(byte_slice_t));
  // bit_code_t* codes = calloc(256, sizeof(bit_code_t)); 
  // cvector_vector_type(bit_code_t) codes = NULL;
  // cvector_reserve(codes, 256);

  tree_extract_codes(root, codes);

  printf("-------------------------\n");
  printf("COMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  printf("BYTE   | FREQUENCY | CODE\n");
  printf("-------------------------\n");
  for(int i = 0; i < unique_byte_count; i++){
    byte_freq_t bf = bf_arr[i];
    printf("%d (%c) | %9.1f | ", bf.byte, bf.byte, (float)(100*bf.freq) / total_byte_count);
    byte_slice_print(codes[bf.byte]);
    printf("\n");
  }
  printf("-------------------------\n");

  printf("1 ");
  // exit(1);

  bit_set_t bit_set;
  // exit(1);
  bit_set_init(&bit_set);

  // printf("here");

  //write 4 bytes -> total bytes count
  unsigned char* bytes = (unsigned char *)&total_byte_count;
  bit_set_write_byte(&bit_set, bytes[0]);
  bit_set_write_byte(&bit_set, bytes[1]);
  bit_set_write_byte(&bit_set, bytes[2]);
  bit_set_write_byte(&bit_set, bytes[3]);
  //write 1 byte -> unique bytes count
  bit_set_write_byte(&bit_set, unique_byte_count);
  //write huffman tree data
  tree_write_to_bitset(root, &bit_set);
  //write compressed data
  for(uint32_t i = 0; i < total_byte_count; i++){
    unsigned char byte = in_buffer[i];
    // bit_code_t code = codes[byte];
    // bit_set_write_code(&bit_set, code);
    byte_slice_t slice = codes[byte];
    bit_set_write_slice(&bit_set, slice);
  }
  bit_set_end_write(&bit_set);

  // cvector_free(bf_arr);
  std::vector<unsigned char> out_buffer = bit_set_extract_bytes(&bit_set);
  //extract to out_buffer from bit_set

  bit_set_free(&bit_set);

  double reduction = 100.f * (1.f - ((double)out_buffer.size() / (double)in_buffer.size())); 
  printf("size after compression: %ld, %.1f%% reduction\n", out_buffer.size(), reduction);
  printf("-------------------------\n");
  
  tree_free(root);
  free(codes);
  // cvector_free(codes);
  return out_buffer;
}