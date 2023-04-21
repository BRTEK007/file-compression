#include "tree.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "node_queue.hpp"

node_t* create_tree(std::vector<byte_freq_t> bf_arr){
  node_queue_t* node_queue = node_queue_create(bf_arr.size());

  //create a leaf node for each symbol
  for(int i = 0; i < bf_arr.size(); i++){
    node_t* node = new node_t();
    // *node = (node_t){
    //   byte: bf_arr[i].byte,
    //   freq: bf_arr[i].freq,
    //   leaf: true
    // };
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
    node_t* node_internal = new node_t();
    // node_t* node_internal = malloc(sizeof(node_t));
    // *node_internal = (node_t){
    //   freq: nodeA->freq + nodeB->freq,
    //   leaf: false,
    //   left: nodeA,
    //   right: nodeB
    // };
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
  // cvector_vector_type(node_t*) stack = NULL;
  std::vector<node_t*> stack;
  
  // cvector_push_back(stack, root->right);
  // cvector_push_back(stack, root->left);
  stack.push_back(root->right);
  stack.push_back(root->left);

  while(stack.size() > 0){
    // node_t* node = stack[cvector_size(stack)-1];
    // cvector_pop_back(stack);
    node_t* node = stack.back();
    stack.pop_back();
    // printf("%d ", stack.size());
    std::cout<<stack.size()<<std::endl;

    if(node == NULL)
      continue;
    
    if(node->leaf){
      // printf("1 %c ", node->byte);
      bit_set_write_bit(bit_set, true);
      bit_set_write_byte(bit_set, node->byte);
    }else{
      // printf("0 ");
      bit_set_write_bit(bit_set, false);
      // cvector_push_back(stack, node->right);
      // cvector_push_back(stack, node->left);
      stack.push_back(root->right);
      stack.push_back(root->left);
    }
  }
  // printf("\n");

  // cvector_free(stack);
}

node_t* tree_read_from_bitset(bit_set_t* bit_set, uint8_t leaf_count){
  node_t* root = new node_t();
  root->leaf = false;
  root->left = NULL;
  root->right = NULL;

  // cvector_vector_type(node_t**) stack = NULL;
  std::vector<node_t**> stack;

  // cvector_push_back(stack, &(root->right));
  // cvector_push_back(stack, &(root->left));
      stack.push_back(&(root->right));
      stack.push_back(&(root->left));

  uint8_t leaves_read = 0;
  while(leaves_read < leaf_count){
    // node_t** node = stack[cvector_size(stack)-1];
    // cvector_pop_back(stack);
    node_t** node = stack.back();
    stack.pop_back();

    bool bit = bit_set_read_bit(bit_set);

    if(!bit){//parent node
      // printf("0 ");
      node_t* new_node = new node_t();
      new_node->leaf = false;
      new_node->left = NULL;
      new_node->right = NULL;
      *node = new_node;
      
      // cvector_push_back(stack, &(new_node->right));
      stack.push_back(&(root->right));
      stack.push_back(&(root->left));
      // cvector_push_back(stack, &(new_node->left));
    }else{//leaf node
      node_t* new_node = new node_t();
      new_node->byte = bit_set_read_byte(bit_set);
      new_node->leaf = true;
      new_node->left = NULL;
      new_node->right = NULL;
      *node = new_node;
      // printf("1 %c ", new_node->byte);
      leaves_read++;
    }
  }

  // cvector_free(stack);
  return root;
}

void tree_extract_codes(node_t* root, byte_slice_t* codes){
  byte_slice_t slice;
  byte_slice_init(&slice);
  tree_extract_codes_rec(root, slice, codes);
}

void tree_extract_codes_rec(node_t* node, byte_slice_t code, byte_slice_t* codes){
  if(node == NULL) return;
  if(node->leaf){
    codes[node->byte] = code;
  }else{
    byte_slice_t slice_left = code;
    byte_slice_t slice_right = code;

    byte_slice_write_bit(&slice_left, false);
    byte_slice_write_bit(&slice_right, true);

    tree_extract_codes_rec(node->left, slice_left, codes);
    
    tree_extract_codes_rec(node->right, slice_right, codes);
  }
}

void tree_extract_leaf_bytes(node_t* node,  std::vector<unsigned char>* bytes){
  if(node == NULL) return;
  if(node->leaf){
    // cvector_push_back(*bytes, node->byte);
    bytes->push_back(node->byte);
    // printf("extracted %c ", node->byte);
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