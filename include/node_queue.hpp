#ifndef NODE_QUEUE_H
#define NODE_QUEUE_H

#include <stdlib.h>

#include "tree.hpp"

class NodeQueue{
  private:
    std::vector<node_t*> nodes;
  public: 
    NodeQueue(size_t size);
    size_t size();
    void push(node_t* node);
    node_t* pop();
};

#endif