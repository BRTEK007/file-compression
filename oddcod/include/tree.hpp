#ifndef TREE_H
#define TREE_H

#include <vector>
#include <array>

#include "byte_freq.hpp"
#include "byte_slice.hpp"
#include "bit_code.hpp"
#include "bit_ostream.hpp"
#include "bit_istream.hpp"

namespace oddcod
{
  // Node of the Code Tree, refrences following nodes, stores byte, leaf, frequency information.
  struct CodeTreeNode
  {
    unsigned char byte;
    bool leaf;
    unsigned freq;
    struct CodeTreeNode *left;
    struct CodeTreeNode *right;
  };

  // Priority queue of code tree nodes ordered by frequency.
  class CodeTreeNodeQueue
  {
  private:
    std::vector<CodeTreeNode *> nodes;

  public:
    CodeTreeNodeQueue(size_t size);
    size_t size();
    void push(CodeTreeNode *node);
    CodeTreeNode *pop();
  };

  // Tree where the leaves hold byte values, and the associated bit code is encoded in the path.
  class CodeTree
  {
    CodeTreeNode *head;
    CodeTreeNode *nodePtr;

  public:
    CodeTree();
    CodeTree(const std::vector<ByteFreq> &bf_arr);
    ~CodeTree();
    void readFrom(std::shared_ptr<BitReader> bitReader, size_t leaf_count);
    void extractCodes(std::array<BitCode, 256> &codes);
    void extractLeafBytes(std::vector<unsigned char> &bytes);
    void writeTo(std::shared_ptr<BitWriter> bitOstream);
    void ptrReset();
    void ptrRight();
    void ptrLeft();
    bool ptrIsLeaf();
    bool ptrIsNull();
    unsigned char ptrReadByte();
  };
};
#endif