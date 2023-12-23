#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

#include "byte_freq.hpp"
#include "byte_slice.hpp"
#include <vector>
#include <array>
#include "bitcode.hpp"
#include "bitostream.hpp"
#include "bitistream.hpp"

struct Node
{
  unsigned char byte;
  bool leaf;
  unsigned freq;
  struct Node *left;
  struct Node *right;
};

class Tree
{
  Node *head;
  Node *nodePtr;

public:
  Tree();
  Tree(const std::vector<ByteFreq> &bf_arr);
  ~Tree();
  void readFrom(BitIstream &bitIstream, size_t leaf_count);
  void extractCodes(std::array<BitCode, 256> &codes);
  void extractLeafBytes(std::vector<unsigned char> &bytes);
  void writeTo(BitOstream &bitOstream);
  void ptrReset();
  void ptrRight();
  void ptrLeft();
  bool ptrIsLeaf();
  unsigned char ptrReadByte();
};

#endif