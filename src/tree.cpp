#include "tree.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "node_queue.hpp"

Tree::Tree(){
  this->head = NULL;
  this->node_ptr = NULL;
}

Tree::~Tree(){
  std::vector<node_t*> stack;
  stack.push_back(this->head);
  while(!stack.empty()){
    node_t* node = stack.back();
    stack.pop_back();
    if(node != NULL){
      stack.push_back(node->right);
      stack.push_back(node->left);
      delete node;
    }
  }
}


void Tree::create_from_bytefreq(const std::vector<byte_freq_t>& bf_arr){
  NodeQueue node_queue(bf_arr.size());

  //create a leaf node for each symbol
  for(int i = 0; i < bf_arr.size(); i++){
    node_t* node = new node_t;
    node->byte = bf_arr[i].byte;
    node->freq = bf_arr[i].freq;
    node->leaf = true;
    node->left = NULL;
    node->right = NULL;
    node_queue.push(node);
  }

  //while more than one node in the queue
  while(node_queue.size() > 1){
    //remove two nodes of highest probability
    node_t* nodeA = node_queue.pop();
    node_t* nodeB = node_queue.pop();
    //create new internal node
    node_t* node_internal = new node_t;
    node_internal->freq = nodeA->freq + nodeB->freq;
    node_internal->leaf = false;
    node_internal->left = nodeA;
    node_internal->right = nodeB;
    node_queue.push(node_internal);
  }
  
  this->head = node_queue.pop();
}

void Tree::extract_codes(std::array<BitCode<256>, 256>& codes){
  std::vector<std::pair<node_t*, BitCode<256>>> stack;
  
  BitCode<256> code;
  stack.push_back({this->head, code});

  while(!stack.empty()){
    auto node_code = stack.back();
    stack.pop_back();

    node_t* node = node_code.first;
    BitCode<256> code = node_code.second;

    if(node != NULL){
      BitCode<256> code_left = code;
      BitCode<256> code_right = code;

      code_left.writeBit(false);
      code_right.writeBit(true);

      stack.push_back({node->right, code_right});
      stack.push_back({node->left, code_left});
      
      if(node->leaf){
        codes[node->byte] = code;
      }
    }
  }
}

void Tree::extract_leaf_bytes(std::vector<unsigned char>& bytes){
  std::vector<node_t*> stack;
  stack.push_back(this->head);

  bytes.clear();

  while(!stack.empty()){
    node_t* node = stack.back();
    stack.pop_back();
    if(node != NULL){
      stack.push_back(node->right);
      stack.push_back(node->left);
      if(node->leaf){
        bytes.push_back(node->byte);
      }
    }
  }

}

void Tree::write_to_bitset(bit_set_t* bit_set){
  std::vector<node_t*> stack;
  
  stack.push_back(this->head->right);
  stack.push_back(this->head->left);

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

void Tree::create_from_bitset(bit_set_t* bit_set, uint8_t leaf_count){
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
  this->head = root;
}

void Tree::ptr_reset(){
  this->node_ptr = this->head;
}

void Tree::ptr_right(){
  this->node_ptr = this->node_ptr->right;
}

void Tree::ptr_left(){
  this->node_ptr = this->node_ptr->left;
}

bool Tree::ptr_is_leaf(){
  return this->node_ptr->leaf;
}

unsigned char Tree::ptr_read_byte(){
  return this->node_ptr->byte;
}