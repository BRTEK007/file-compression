#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CVECTOR_LOGARITHMIC_GROWTH
#include "c-vector/cvector.h"

typedef struct bit_code_t{
  unsigned char code;
  int len;
} bit_code_t;

void bit_code_print(bit_code_t bc){
  int mask = 1u;
  char str[bc.len + 1];
  for(int i = 0; i < bc.len; i++){
    str[i] = mask & bc.code ? '1' : '0';
    mask = mask << 1;
  }
  str[bc.len] = '\0';
  printf("%s", str);
}

bit_code_t bit_code_empty(){
  bit_code_t bc;
  bc.len = 0;
  bc.code = 0;
  return bc;
}

bit_code_t bit_code_add_one(bit_code_t bc){
  bit_code_t nbc = bc;
  nbc.code = nbc.code | (1u << nbc.len);
  nbc.len++;
  return nbc;
}

bit_code_t bit_code_add_zero(bit_code_t bc){
  bit_code_t nbc = bc;
  nbc.len++;
  return nbc;
}

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

typedef struct node_queue_t{
  node_t** nodes;
} node_queue_t;

node_queue_t* node_queue_create(size_t size){
  node_queue_t* nq = malloc(sizeof(node_queue_t));
  cvector_vector_type(node_t*) nodes = NULL;
  cvector_reserve(nodes, size);
  nq->nodes = nodes;
  return nq;
}

size_t node_queue_size(node_queue_t* nq){
  return cvector_size(nq->nodes);
}

void node_queue_insert(node_queue_t* nq, node_t* node){
  cvector_push_back(nq->nodes, node); 
}

node_t* node_queue_pop(node_queue_t* nq){
  node_t* node = nq->nodes[0];
  int index = 0;
  for(int i = 0; i < cvector_size(nq->nodes); i++){
    if(nq->nodes[i]->freq < node->freq){
      node = nq->nodes[i];
      index = i;
    }
  }
  cvector_erase(nq->nodes, index);
  return node;
}

void node_queue_free(node_queue_t* nq){
  cvector_free(nq);
  free(nq);
}

int cmp_byte_freq (const void * a, const void * b) {
   return ( ((byte_freq_t*)a)->freq - ((byte_freq_t*)b)->freq );
}

byte_freq_t* get_byte_freq_arr(unsigned char* input){ 
  byte_freq_t* arr = calloc(256, sizeof(byte_freq_t));//256 bytes, count their frequency
  
  for(int i = 0; i < 256; i++){//init array
    arr[i].byte = (unsigned char)i;
    arr[i].freq = 0;
  }

  for(int i = 0; i < cvector_size(input); i++){//go through bytes in input file and increment their freq in array
    unsigned char index = input[i];
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

  cvector_vector_type(byte_freq_t) bf_arr = NULL;
  cvector_reserve(bf_arr, size);

  //copy bytes with atleast 1 occurance to new array
  for(int i = 0; i < size; i++){
    cvector_push_back(bf_arr, arr[zeros+i]);
  }

  free(arr);
  
  return bf_arr;
}

unsigned char* get_input_buffer(const char* filename){ 
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

  cvector_vector_type(unsigned char) vec = NULL;

  cvector_reserve(vec, filelen);

  for(int i = 0; i < filelen; i++){
    cvector_push_back(vec, buffer[i]);
  }

  free(buffer);
  return vec;
}

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
  // node_queue_free(node_queue);
  return root;
}

void tree_traversal(node_t* node, bit_code_t bc){
  if(node == NULL) return;
  if(node->leaf){
    printf("%c: ", node->byte);
    bit_code_print(bc);
    printf("\n");
  }else{
    tree_traversal(node->left, bit_code_add_zero(bc));
    
    tree_traversal(node->right, bit_code_add_one(bc));
  }
}

void tree_free(node_t* node){
  if(node == NULL) return;
  tree_free(node->left);
  tree_free(node->right);
  free(node);
}

int main(int argc, char** argv){
  if(argc != 2){
    fprintf(stderr, "ERROR: wrong arguments!\n");
    exit(1);
  }

  unsigned char* buffer = get_input_buffer(argv[1]); 
  byte_freq_t* bf_arr = get_byte_freq_arr(buffer);

  // for(int i = 0; i < cvector_size(bf_arr); i++){
  //   byte_freq_t bf = bf_arr[i];
  //   printf("%c : %d\n", bf.byte, bf.freq);
  // }

  // node_t* root = create_tree(bf_arr);
  // bit_code_t bc = bit_code_empty();
  // tree_traversal(root, bc);
  // tree_free(root);

  cvector_free(buffer);
  cvector_free(bf_arr);
  return 0;
}

