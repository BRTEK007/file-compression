#include "node_queue.hpp"

NodeQueue::NodeQueue(size_t size)
{
  this->nodes.reserve(size);
}

size_t NodeQueue::size()
{
  return this->nodes.size();
}

void NodeQueue::push(Node *node)
{
  this->nodes.push_back(node);
}

Node *NodeQueue::pop()
{
  Node *node = this->nodes[0];
  int index = 0;
  for (size_t i = 0; i < this->nodes.size(); i++)
  {
    if (this->nodes[i]->freq < node->freq)
    {
      node = this->nodes[i];
      index = i;
    }
  }
  this->nodes.erase(this->nodes.begin() + index);
  return node;
}