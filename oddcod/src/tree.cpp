#include "tree.hpp"

// node queue
namespace oddcod{
  CodeTreeNodeQueue::CodeTreeNodeQueue(size_t size)
  {
    this->nodes.reserve(size);
  }

  size_t CodeTreeNodeQueue::size()
  {
    return this->nodes.size();
  }

  void CodeTreeNodeQueue::push(CodeTreeNode *node)
  {
    this->nodes.push_back(node);
  }

  CodeTreeNode *CodeTreeNodeQueue::pop()
  {
    CodeTreeNode *node = this->nodes[0];
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
};

// tree
namespace oddcod
{
  CodeTree::CodeTree()
  {
    this->head = NULL;
    this->nodePtr = NULL;
  }

  CodeTree::~CodeTree()
  {
    std::vector<CodeTreeNode *> stack;
    stack.push_back(this->head);
    while (!stack.empty())
    {
      CodeTreeNode *node = stack.back();
      stack.pop_back();
      if (node != NULL)
      {
        stack.push_back(node->right);
        stack.push_back(node->left);
        delete node;
      }
    }
  }

  CodeTree::CodeTree(const std::vector<ByteFreq> &bf_arr)
  {
    CodeTreeNodeQueue queue(bf_arr.size());

    // create a leaf node for each symbol
    for (size_t i = 0; i < bf_arr.size(); i++)
    {
      CodeTreeNode *node = new CodeTreeNode;
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
      CodeTreeNode *nodeA = queue.pop();
      CodeTreeNode *nodeB = queue.pop();
      // create new internal node
      CodeTreeNode *nodeInternal = new CodeTreeNode;
      nodeInternal->freq = nodeA->freq + nodeB->freq;
      nodeInternal->leaf = false;
      nodeInternal->left = nodeA;
      nodeInternal->right = nodeB;
      queue.push(nodeInternal);
    }

    this->head = queue.pop();
  }

  void CodeTree::extractCodes(std::array<BitCode, 256> &codes)
  {
    std::vector<std::pair<CodeTreeNode *, BitCode>> stack;

    BitCode code;
    stack.push_back({this->head, code});

    while (!stack.empty())
    {
      auto nodeCode = stack.back();
      stack.pop_back();

      CodeTreeNode *node = nodeCode.first;
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

  void CodeTree::extractLeafBytes(std::vector<unsigned char> &bytes)
  {
    std::vector<CodeTreeNode *> stack;
    stack.push_back(this->head);

    bytes.clear();

    while (!stack.empty())
    {
      CodeTreeNode *node = stack.back();
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

  void CodeTree::writeTo(std::shared_ptr<BitWriter> bitOstream)
  {
    std::vector<CodeTreeNode *> stack;

    stack.push_back(this->head->right);
    stack.push_back(this->head->left);

    while (!stack.empty())
    {
      CodeTreeNode *node = stack.back();
      stack.pop_back();

      if (!node)
        continue;

      if (node->leaf)
      {
        bitOstream->write(true);
        bitOstream->write(node->byte);
      }
      else
      {
        bitOstream->write(false);
        stack.push_back(node->right);
        stack.push_back(node->left);
      }
    }
  }

  void CodeTree::readFrom(std::shared_ptr<BitReader> bitIstream, size_t leaf_count)
  {
    CodeTreeNode *root = new CodeTreeNode();
    root->leaf = false;
    root->left = NULL;
    root->right = NULL;

    std::vector<CodeTreeNode **> stack;

    stack.push_back(&(root->right));
    stack.push_back(&(root->left));

    size_t leavesRead = 0;
    while (leavesRead < leaf_count)
    {
      CodeTreeNode **node = stack.back();
      stack.pop_back();

      bool bit = bitIstream->readBit(); // TODO check eof, and throw

      if (!bit)
      { // parent node
        CodeTreeNode *newNode = new CodeTreeNode();
        newNode->leaf = false;
        newNode->left = NULL;
        newNode->right = NULL;
        *node = newNode;

        stack.push_back(&(newNode->right));
        stack.push_back(&(newNode->left));
      }
      else
      { // leaf node
        CodeTreeNode *newNode = new CodeTreeNode();
        newNode->byte = bitIstream->readByte();
        newNode->leaf = true;
        newNode->left = NULL;
        newNode->right = NULL;
        *node = newNode;
        leavesRead++;
      }
    }
    this->head = root;
  }

  void CodeTree::ptrReset()
  {
    this->nodePtr = this->head;
  }

  void CodeTree::ptrRight()
  {
    this->nodePtr = this->nodePtr->right;
  }

  void CodeTree::ptrLeft()
  {
    this->nodePtr = this->nodePtr->left;
  }

  bool CodeTree::ptrIsLeaf()
  {
    return this->nodePtr->leaf;
  }

  bool CodeTree::ptrIsNull()
  {
    return this->nodePtr == NULL;
  }

  unsigned char CodeTree::ptrReadByte()
  {
    return this->nodePtr->byte;
  }
};