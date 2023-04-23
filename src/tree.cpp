#include "tree.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "node_queue.hpp"

node_t* create_tree(std::vector<byte_freq_t> bf_arr){
  node_queue_t* node_queue = node_queue_create(bf_arr.size());

  //create a leaf node for each symbol
  for(int i = 0; i < bf_arr.size(); i++){
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->byte = bf_arr[i].byte;
    node->freq = bf_arr[i].freq;
    node->leaf = true;
    node->left = NULL;
    node->right = NULL;
    node_queue_insert(node_queue, node);
  }

  //while more than one node in the queue
  while(node_queue_size(node_queue) > 1){
    //remove two nodes of highest probability
    node_t* nodeA = node_queue_pop(node_queue);
    node_t* nodeB = node_queue_pop(node_queue);
    //create new internal node
    node_t* node_internal = (node_t*)malloc(sizeof(node_t));
    node_internal->freq = nodeA->freq + nodeB->freq;
    node_internal->leaf = false;
    node_internal->left = nodeA;
    node_internal->right = nodeB;
    node_queue_insert(node_queue, node_internal);
  }
  
  node_t* root = node_queue_pop(node_queue);
  node_queue_free(node_queue);
  return root;
}

void tree_write_to_bitset(node_t* root, bit_set_t* bit_set){
  std::vector<node_t*> stack;
  
  stack.push_back(root->right);
  stack.push_back(root->left);

  while(!stack.empty()){
    node_t* node = stack.back();
    stack.pop_back();

    if(!node)
      continue;
    
    
    if(node->leaf){
      bit_set_write_bit(bit_set, true);
      bit_set_write_byte(bit_set, node->byte);
    }else{
      bit_set_write_bit(bit_set, false);
      stack.push_back(node->right);
      stack.push_back(node->left);
    }
  }
}

node_t* tree_read_from_bitset(bit_set_t* bit_set, uint8_t leaf_count){
  node_t* root = new node_t();
  root->leaf = false;
  root->left = NULL;
  root->right = NULL;

  std::vector<node_t**> stack;

  stack.push_back(&(root->right));
  stack.push_back(&(root->left));

  uint8_t leaves_read = 0;
  while(leaves_read < leaf_count){
    node_t** node = stack.back();
    stack.pop_back();

    bool bit = bit_set_read_bit(bit_set);

    if(!bit){//parent node
      node_t* new_node = new node_t();
      new_node->leaf = false;
      new_node->left = NULL;
      new_node->right = NULL;
      *node = new_node;
      
      stack.push_back(&(new_node->right));
      stack.push_back(&(new_node->left));
    }else{//leaf node
      node_t* new_node = new node_t();
      new_node->byte = bit_set_read_byte(bit_set);
      new_node->leaf = true;
      new_node->left = NULL;
      new_node->right = NULL;
      *node = new_node;
      leaves_read++;
    }
  }

  return root;
}

void tree_extract_codes(node_t* root, BitCode* codes){
  BitCode code;
  tree_extract_codes_rec(root, code, codes);
}

void tree_extract_codes_rec(node_t* node, BitCode code, BitCode* codes){
  if(node == NULL) return;
  if(node->leaf){
    codes[node->byte] = code;
  }else{
    BitCode code_left = code;
    BitCode code_right = code;

    code_left.writeBit(false);
    code_right.writeBit(true);

    tree_extract_codes_rec(node->left, code_left, codes);
    
    tree_extract_codes_rec(node->right, code_right, codes);
  }
}

void tree_extract_leaf_bytes(node_t* node,  std::vector<unsigned char>* bytes){
  if(node == NULL) return;
  if(node->leaf){
    bytes->push_back(node->byte);
  }else{
    tree_extract_leaf_bytes(node->left, bytes);
    tree_extract_leaf_bytes(node->right, bytes);
  }
}

void tree_free(node_t* node){
  if(node == NULL) return;
  tree_free(node->left);
  tree_free(node->right);
  free(node);
}