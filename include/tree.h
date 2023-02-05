#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

#include "byte_freq.h"
#include "byte_slice.h"
#include "bit_set.h"

typedef struct node_t{
  unsigned char byte;
  bool leaf;
  int freq;
  struct node_t* left;
  struct node_t* right;
} node_t;

node_t* create_tree(byte_freq_t* bf_arr);

void tree_extract_codes(node_t* root, byte_slice_t* codes);

void tree_extract_codes_rec(node_t* root, byte_slice_t slice, byte_slice_t* codes);

void tree_free(node_t* node);

void tree_write_to_bitset(node_t* root, bit_set_t* bit_set);

// void tree_traversal_recon(node_t* node);

// void tree_recon(unsigned char* data);

#endif