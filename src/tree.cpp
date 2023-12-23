#include "tree.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "node_queue.hpp"

Tree::Tree()
{
  this->head = NULL;
  this->node_ptr = NULL;
}

Tree::~Tree()
{
  std::vector<Node *> stack;
  stack.push_back(this->head);
  while (!stack.empty())
  {
    Node *node = stack.back();
    stack.pop_back();
    if (node != NULL)
    {
      stack.push_back(node->right);
      stack.push_back(node->left);
      delete node;
    }
  }
}

Tree::Tree(const std::vector<ByteFreq> &bf_arr)
{
  NodeQueue node_queue(bf_arr.size());

  // create a leaf node for each symbol
  for (size_t i = 0; i < bf_arr.size(); i++)
  {
    Node *node = new Node;
    node->byte = bf_arr[i].byte;
    node->freq = bf_arr[i].freq;
    node->leaf = true;
    node->left = NULL;
    node->right = NULL;
    node_queue.push(node);
  }

  // while more than one node in the queue
  while (node_queue.size() > 1)
  {
    // remove two nodes of highest probability
    Node *nodeA = node_queue.pop();
    Node *nodeB = node_queue.pop();
    // create new internal node
    Node *node_internal = new Node;
    node_internal->freq = nodeA->freq + nodeB->freq;
    node_internal->leaf = false;
    node_internal->left = nodeA;
    node_internal->right = nodeB;
    node_queue.push(node_internal);
  }

  this->head = node_queue.pop();
}

void Tree::extractCodes(std::array<BitCode, 256> &codes)
{
  std::vector<std::pair<Node *, BitCode>> stack;

  BitCode code;
  stack.push_back({this->head, code});

  while (!stack.empty())
  {
    auto node_code = stack.back();
    stack.pop_back();

    Node *node = node_code.first;
    BitCode code = node_code.second;

    if (node != NULL)
    {
      BitCode code_left = code;
      BitCode code_right = code;

      code_left.writeBit(false);
      code_right.writeBit(true);

      stack.push_back({node->right, code_right});
      stack.push_back({node->left, code_left});

      if (node->leaf)
      {
        codes[node->byte] = code;
      }
    }
  }
}

void Tree::extractLeafBytes(std::vector<unsigned char> &bytes)
{
  std::vector<Node *> stack;
  stack.push_back(this->head);

  bytes.clear();

  while (!stack.empty())
  {
    Node *node = stack.back();
    stack.pop_back();
    if (node != NULL)
    {
      stack.push_back(node->right);
      stack.push_back(node->left);
      if (node->leaf)
      {
        bytes.push_back(node->byte);
      }
    }
  }
}

void Tree::writeTo(BitOstream &bitOstream)
{
  std::vector<Node *> stack;

  stack.push_back(this->head->right);
  stack.push_back(this->head->left);

  while (!stack.empty())
  {
    Node *node = stack.back();
    stack.pop_back();

    if (!node)
      continue;

    if (node->leaf)
    {
      bitOstream.write(true);
      bitOstream.write(node->byte);
    }
    else
    {
      bitOstream.write(false);
      stack.push_back(node->right);
      stack.push_back(node->left);
    }
  }
}

void Tree::readFrom(BitIstream &bitIstream, size_t leaf_count)
{
  Node *root = new Node();
  root->leaf = false;
  root->left = NULL;
  root->right = NULL;

  std::vector<Node **> stack;

  stack.push_back(&(root->right));
  stack.push_back(&(root->left));

  size_t leaves_read = 0;
  while (leaves_read < leaf_count)
  {
    Node **node = stack.back();
    stack.pop_back();

    bool bit = bitIstream.readBit();

    if (!bit)
    { // parent node
      Node *new_node = new Node();
      new_node->leaf = false;
      new_node->left = NULL;
      new_node->right = NULL;
      *node = new_node;

      stack.push_back(&(new_node->right));
      stack.push_back(&(new_node->left));
    }
    else
    { // leaf node
      Node *new_node = new Node();
      new_node->byte = bitIstream.readByte();
      new_node->leaf = true;
      new_node->left = NULL;
      new_node->right = NULL;
      *node = new_node;
      leaves_read++;
    }
  }
  this->head = root;
}

void Tree::ptrReset()
{
  this->node_ptr = this->head;
}

void Tree::ptrRight()
{
  this->node_ptr = this->node_ptr->right;
}

void Tree::ptrLeft()
{
  this->node_ptr = this->node_ptr->left;
}

bool Tree::ptrIsLeaf()
{
  return this->node_ptr->leaf;
}

unsigned char Tree::ptrReadByte()
{
  return this->node_ptr->byte;
}