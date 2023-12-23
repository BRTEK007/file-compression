#include "tree.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "node_queue.hpp"

Tree::Tree()
{
  this->head = NULL;
  this->nodePtr = NULL;
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
  NodeQueue queue(bf_arr.size());

  // create a leaf node for each symbol
  for (size_t i = 0; i < bf_arr.size(); i++)
  {
    Node *node = new Node;
    node->byte = bf_arr[i].byte;
    node->freq = bf_arr[i].freq;
    node->leaf = true;
    node->left = NULL;
    node->right = NULL;
    queue.push(node);
  }

  // while more than one node in the queue
  while (queue.size() > 1)
  {
    // remove two nodes of highest probability
    Node *nodeA = queue.pop();
    Node *nodeB = queue.pop();
    // create new internal node
    Node *nodeInternal = new Node;
    nodeInternal->freq = nodeA->freq + nodeB->freq;
    nodeInternal->leaf = false;
    nodeInternal->left = nodeA;
    nodeInternal->right = nodeB;
    queue.push(nodeInternal);
  }

  this->head = queue.pop();
}

void Tree::extractCodes(std::array<BitCode, 256> &codes)
{
  std::vector<std::pair<Node *, BitCode>> stack;

  BitCode code;
  stack.push_back({this->head, code});

  while (!stack.empty())
  {
    auto nodeCode = stack.back();
    stack.pop_back();

    Node *node = nodeCode.first;
    BitCode code = nodeCode.second;

    if (node != NULL)
    {
      BitCode codeLeft = code;
      BitCode codeRight = code;

      codeLeft.writeBit(false);
      codeRight.writeBit(true);

      stack.push_back({node->right, codeRight});
      stack.push_back({node->left, codeLeft});

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

  size_t leavesRead = 0;
  while (leavesRead < leaf_count)
  {
    Node **node = stack.back();
    stack.pop_back();

    bool bit = bitIstream.readBit();

    if (!bit)
    { // parent node
      Node *newNode = new Node();
      newNode->leaf = false;
      newNode->left = NULL;
      newNode->right = NULL;
      *node = newNode;

      stack.push_back(&(newNode->right));
      stack.push_back(&(newNode->left));
    }
    else
    { // leaf node
      Node *newNode = new Node();
      newNode->byte = bitIstream.readByte();
      newNode->leaf = true;
      newNode->left = NULL;
      newNode->right = NULL;
      *node = newNode;
      leavesRead++;
    }
  }
  this->head = root;
}

void Tree::ptrReset()
{
  this->nodePtr = this->head;
}

void Tree::ptrRight()
{
  this->nodePtr = this->nodePtr->right;
}

void Tree::ptrLeft()
{
  this->nodePtr = this->nodePtr->left;
}

bool Tree::ptrIsLeaf()
{
  return this->nodePtr->leaf;
}

unsigned char Tree::ptrReadByte()
{
  return this->nodePtr->byte;
}