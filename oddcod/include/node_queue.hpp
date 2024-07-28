#ifndef NODE_QUEUE_H
#define NODE_QUEUE_H

#include <stdlib.h>

#include "tree.hpp"

class NodeQueue
{
private:
  std::vector<Node *> nodes;

public:
  NodeQueue(size_t size);
  size_t size();
  void push(Node *node);
  Node *pop();
};

#endif