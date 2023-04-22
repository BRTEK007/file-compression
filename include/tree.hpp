#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

#include "byte_freq.hpp"
#include "byte_slice.hpp"
#include "bit_set.hpp"
#include <vector>

typedef struct node_t{
  unsigned char byte;
  bool leaf;
  int freq;
  struct node_t* left;
  struct node_t* right;
} node_t;

node_t* create_tree(std::vector<byte_freq_t> bf_arr);

void tree_extract_codes(node_t* root, byte_slice_t* codes);

void tree_extract_codes_rec(node_t* root, byte_slice_t slice, byte_slice_t* codes);

void tree_extract_leaf_bytes(node_t* node, std::vector<unsigned char>* bytes);

void tree_free(node_t* node);

void tree_write_to_bitset(node_t* root, bit_set_t* bit_set);

node_t* tree_read_from_bitset(bit_set_t* bit_set, uint8_t leaf_count);

#endif