#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define UNI_VEC_IMPLEMENTATION
#include "uni_vec.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


typedef struct byte_freq_t{
  unsigned char byte;
  int freq;
} byte_freq_t;

typedef struct node_t{
  unsigned char byte;
  bool leaf;
  int freq;
  struct node_t* left;
  struct node_t* right;
} node_t;

int cmp_byte_freq (const void * a, const void * b) {
   return ( ((byte_freq_t*)a)->freq - ((byte_freq_t*)b)->freq );
}

uni_vec_t* get_byte_freq_arr(uni_vec_t* input){ 
  byte_freq_t* arr = calloc(256, sizeof(byte_freq_t));//256 bytes, count their frequency
  
  for(int i = 0; i < 256; i++){//init array
    arr[i].byte = (unsigned char)i;
    arr[i].freq = 0;
  }

  for(size_t i = 0; i < input->size; i++){//go through bytes in input file and increment their freq in array
    unsigned char index = *((unsigned char*)uni_vec_get(input, i));
    arr[index].freq++;
  }

  qsort(arr, 256, sizeof(byte_freq_t), cmp_byte_freq);

  //sort array, count zeros (bytes that didnt exist in file)

  byte_freq_t* arr_ptr = arr;
  int zeros = 0;
  while(arr_ptr->freq == 0){
    arr_ptr++;
    zeros++;
  }

  size_t size = 256 - zeros;

  uni_vec_t* bf_arr = uni_vec_create(size, sizeof(byte_freq_t));

  //copy bytes with atleast 1 occurance to new array
  uni_vec_push_back_array(bf_arr, arr+zeros, size);

  free(arr);
  
  return bf_arr;
}

uni_vec_t* get_input_buffer(const char* filename){ 
  FILE* inputFile;
  
  inputFile = fopen(filename, "rb");
  if(inputFile == NULL){
    fprintf(stderr, "ERROR: could not open file\n");
    exit(2);
  }
  
  fseek(inputFile, 0, SEEK_END);        // Jump to the end of the file
  size_t filelen = ftell(inputFile);      // Get the current byte offset in the file
  rewind(inputFile);                      // Jump back to the beginning of the file

  unsigned char* buffer = calloc(filelen, sizeof(unsigned char)); // Enough memory for the file
  fread(buffer, filelen, 1, inputFile); // Read in the entire file 

  fclose(inputFile); // Close the file

  uni_vec_t* bb = uni_vec_create(filelen, sizeof(unsigned char));
  uni_vec_push_back_array(bb, buffer, filelen);

  free(buffer);
  return bb;
}

node_t create_tree(uni_vec_t* bf_arr){
  uni_vec_t* nodes = uni_vec_create(bf_arr->size, sizeof(node_t));
  
  while(bf_arr->size > 0){
    byte_freq_t bf;
    uni_vec_extract(bf_arr, 0, &bf);
    
    if(nodes->size < 2){//create leaf node with byte 
      node_t node = (node_t){
        byte: bf.byte, 
        freq: bf.freq,
        leaf: true,
        left: NULL,
        right: NULL};
      uni_vec_push_back(nodes, &node);
      // printf("new leaf: %c %i \n", node.byte, node.freq);
      uni_vec_pop_front(bf_arr);
    }else{
     node_t* first_node = (node_t*)uni_vec_get(nodes, 0);      
     node_t* second_node = (node_t*)uni_vec_get(nodes, 1);
     if(first_node->freq + second_node->freq < bf.freq){//connect first 2 nodes
      node_t node = (node_t){
        byte: 0, 
        freq: first_node->freq + second_node->freq,
        leaf: false,
        left: first_node,
        right: second_node};
      uni_vec_pop_front(nodes);
      uni_vec_pop_front(nodes);
      uni_vec_push_front(nodes, &node);
      // printf("new root: %c %i \n", node.byte, node.freq);
     }else{//add another leaf node
      node_t node = (node_t){
        byte: bf.byte, 
        freq: bf.freq,
        leaf: true,
        left: NULL,
        right: NULL};

      // printf("new leaf: %c %i \n", node.byte, node.freq);
      uni_vec_push_back(nodes, &node);
      uni_vec_pop_front(bf_arr);
     }
    }
  }

  while(nodes->size != 1){
    node_t* first_node = (node_t*)uni_vec_get(nodes, 0);      
    node_t* second_node = (node_t*)uni_vec_get(nodes, 1);
    node_t node = (node_t){
        byte: 0, 
        freq: first_node->freq + second_node->freq,
        leaf: false,
        left: first_node,
        right: second_node};
      uni_vec_pop_front(nodes);
      uni_vec_pop_front(nodes);
      uni_vec_push_front(nodes, &node);
      // printf("new root: %c %i \n", node.byte, node.freq);
  }

  node_t root;
  uni_vec_extract(nodes, 0, &root);
  uni_vec_free(nodes);

  return root;
}

void tree_traversal(node_t* node, unsigned char code){
  if(node == NULL) return;
  if(node->leaf){
    printf("%c: "BYTE_TO_BINARY_PATTERN"\n", node->byte, BYTE_TO_BINARY(5));
  }else{
    tree_traversal(node->left, (code << 1) & 0);
    tree_traversal(node->right, (code << 1) & 1);
  }
}

int main(int argc, char** argv){
  if(argc != 2){
    fprintf(stderr, "ERROR: wrong arguments!\n");
    exit(1);
  }
  
  uni_vec_t* buffer = get_input_buffer(argv[1]);
  uni_vec_t* bf_arr = get_byte_freq_arr(buffer);

  for(int i = 0; i < bf_arr->size; i++){
    byte_freq_t* bf = ((byte_freq_t*)uni_vec_get(bf_arr, i));
    printf("%c : %d\n", bf->byte, bf->freq);
  }

  node_t root = create_tree(bf_arr);

  tree_traversal(&root, 0);

  uni_vec_free(buffer);
  uni_vec_free(bf_arr); 
  return 0;
}

