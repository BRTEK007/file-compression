#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

#include "byte_freq.h"
#include "bitcode.h"

typedef struct node_t{
  unsigned char byte;
  bool leaf;
  int freq;
  struct node_t* left;
  struct node_t* right;
} node_t;

node_t* create_tree(byte_freq_t* bf_arr);

void tree_traversal(node_t* node, bit_code_t bc, bit_code_t* bit_codes);

void tree_free(node_t* node);

void tree_traversal_recon(node_t* node);

void tree_recon(unsigned char* data);

#endif