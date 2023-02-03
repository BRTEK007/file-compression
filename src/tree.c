#include "tree.h"

#include <stdlib.h>
#include <stdio.h>

#include "node_queue.h"

node_t* create_tree(byte_freq_t* bf_arr){
  node_queue_t* node_queue = node_queue_create(cvector_size(bf_arr));

  //create a leaf node for each symbol
  for(int i = 0; i < cvector_size(bf_arr); i++){
    node_t* node = malloc(sizeof(node_t));
    *node = (node_t){
      byte: bf_arr[i].byte,
      freq: bf_arr[i].freq,
      leaf: true
    };
    node_queue_insert(node_queue, node);
  }

  //while more than one node in the queue
  while(node_queue_size(node_queue) > 1){
    //remove two nodes of highest probability
    node_t* nodeA = node_queue_pop(node_queue);
    node_t* nodeB = node_queue_pop(node_queue);
    //create new internal node
    node_t* node_internal = malloc(sizeof(node_t));
    *node_internal = (node_t){
      freq: nodeA->freq + nodeB->freq,
      leaf: false,
      left: nodeA,
      right: nodeB
    };
    node_queue_insert(node_queue, node_internal);
  }
  
  node_t* root = node_queue_pop(node_queue);
  node_queue_free(node_queue);
  return root;
}

void tree_traversal(node_t* node, bit_code_t bc, bit_code_t* bit_codes){
  if(node == NULL) return;
  if(node->leaf){
    printf("%c: ", node->byte);
    bit_code_print(bc);
    printf("\n");
    bit_codes[node->byte] = bc;
  }else{
    tree_traversal(node->left, bit_code_add_zero(bc), bit_codes);
    
    tree_traversal(node->right, bit_code_add_one(bc), bit_codes);
  }
}

void tree_free(node_t* node){
  if(node == NULL) return;
  tree_free(node->left);
  tree_free(node->right);
  free(node);
}