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

// void tree_traversal(node_t* node, bit_code_t bc, bit_code_t* bit_codes){
//   if(node == NULL) return;
//   if(node->leaf){
//     printf("%c: ", node->byte);
//     bit_code_print(bc);
//     printf("\n");
//     bit_codes[node->byte] = bc;
//   }else{
//     tree_traversal(node->left, bit_code_add_zero(bc), bit_codes);
    
//     tree_traversal(node->right, bit_code_add_one(bc), bit_codes);
//   }
// }

// void tree_traversal_recon(node_t* root){
//   cvector_vector_type(node_t*) stack = NULL;
  
//   cvector_push_back(stack, root->right);
//   cvector_push_back(stack, root->left);

//   bitstack_t data;
//   bitstack_init(&data);

//   while(cvector_size(stack) > 0){
//     node_t* node = stack[cvector_size(stack)-1];
//     cvector_pop_back(stack);
//     if(node == NULL)
//       continue;
    
//     if(node->leaf){
//       // printf("1 %c ", node->byte);
//       bitstack_push_one(&data);
//       bitstack_push_byte(&data, node->byte);
//     }else{
//       // printf("0 ");
//       bitstack_push_zero(&data);
//       cvector_push_back(stack, node->right);
//       cvector_push_back(stack, node->left);
//     }
//   }
//   printf("\n");

//   // tree_recon(data);
//   // bitstack_print_format(&data);

//   cvector_free(stack);
// }

// void tree_recon(unsigned char* data){
//   node_t* root = malloc(sizeof(node_t));
//   root->leaf = false;
//   root->left = NULL;
//   root->right = NULL;

//   bit_code_t* bit_codes = calloc(256, sizeof(bit_code_t));
  
//   bit_code_t bc = bit_code_empty();

//   cvector_vector_type(node_t**) stack = NULL;

//   cvector_push_back(stack, &(root->right));
//   cvector_push_back(stack, &(root->left));

//   int data_id = 0;

//   while(data_id < cvector_size(data)){
//     unsigned char byte = data[data_id];

//     node_t** node = stack[cvector_size(stack)-1];
//     cvector_pop_back(stack);

//     if(byte == '0'){//parent node
//       node_t* new_node = malloc(sizeof(node_t));
//       new_node->leaf = false;
//       new_node->left = NULL;
//       new_node->right = NULL;
//       *node = new_node;
      
//       cvector_push_back(stack, &(new_node->right));
//       cvector_push_back(stack, &(new_node->left));
//       data_id++;
//     }else if(byte == '1'){//leaf node
//       node_t* new_node = malloc(sizeof(node_t));
//       new_node->byte = data[data_id+1];
//       new_node->leaf = true;
//       new_node->left = NULL;
//       new_node->right = NULL;
//       *node = new_node;
//       data_id+=2;
//     }
//   }

//   cvector_free(stack);

//   tree_traversal(root, bc, bit_codes);
  
//   free(bit_codes);

//   tree_free(root);
// }

void tree_extract_codes(node_t* root, byte_slice_t* codes){
byte_slice_t slice;
byte_slice_init(&slice);
tree_extract_codes_rec(root, slice, codes);
}

void tree_extract_codes_rec(node_t* node, byte_slice_t slice, byte_slice_t* codes){
  if(node == NULL) return;
  if(node->leaf){
    codes[node->byte] = slice;
  }else{
    byte_slice_t slice_left = slice;
    byte_slice_t slice_right = slice;

    byte_slice_write_bit(&slice_left, false);
    byte_slice_write_bit(&slice_right, true);

    tree_extract_codes_rec(node->left, slice_left, codes);
    
    tree_extract_codes_rec(node->right, slice_right, codes);
  }
}

void tree_free(node_t* node){
  if(node == NULL) return;
  tree_free(node->left);
  tree_free(node->right);
  free(node);
}