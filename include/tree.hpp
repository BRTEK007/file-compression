#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

#include "byte_freq.hpp"
#include "byte_slice.hpp"
#include "bit_set.hpp"
#include <vector>
#include <array>
#include "bit_code.hpp"

typedef struct node_t{
  unsigned char byte;
  bool leaf;
  int freq;
  struct node_t* left;
  struct node_t* right;
} node_t;

class Tree{
    node_t* head;
    node_t* node_ptr;
  public:
    Tree();
    ~Tree();
    void create_from_bytefreq(const std::vector<byte_freq_t>& bf_arr);
    void extract_codes(std::array<BitCode<256>, 256>& codes);
    void extract_leaf_bytes(std::vector<unsigned char>& bytes);
    void write_to_bitset(bit_set_t* bit_set);
    void create_from_bitset(bit_set_t* bit_set, uint8_t leaf_count);
    void ptr_reset();
    void ptr_right();
    void ptr_left();
    bool ptr_is_leaf();
    unsigned char ptr_read_byte();
};

#endif