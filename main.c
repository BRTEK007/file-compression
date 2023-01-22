#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define UNI_VEC_IMPLEMENTATION
#include "uni_vec.h"

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
  byte_freq_t* cropped_arr = calloc(size, sizeof(byte_freq_t));
  uni_vec_push_array(bf_arr, arr+zeros, size);

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
  uni_vec_push_array(bb, buffer, filelen);

  free(buffer);
  return bb;
}

// void huffman(byte_freq_arr_t* bf_arr){
//   node_t nodes[bf_arr->size];
//   int nodes_size = 0;

//   while(bf_arr->size > 0){
//     if(nodes_size < 2){//create leaf node with byte

//     }
//   }
// /*
// arr = [
//   {byte: F, freq: 1},
//   {byte: M, freq: 1},
//   {byte: G, freq: 4},
//   {byte: X, freq: 6},
//   {byte: A, freq: 10},
// ];

// nodes = [];

// while(arr.len > 0){
//   if(nodes.len < 2){
//     nodes.push({byte: arr[0].byte, freq: arr[0].freq, leaf: true, left: NULL, right: NULL});
//     arr.pop_front();
//   }else if(nodes[0].freq + nodes[1].freq < arr[0].freq){
//     let new_node = {byte: 0, leaf: false, freq: nodes[0].freq + nodes[1].freq, left: nodes[0], right: nodes[1]};
//     nodes.pop();
//     nodes.pop();
//     nodes.push(new_node);
//   }else{
//     nodes.push({byte: arr[0].byte, freq: arr[0].freq, leaf: true, left: NULL, right: NULL});
//     arr.pop_front();
//   }
// }

// root = {leaf: false, left: nodes[0], right: nodes[]};

// some tree traversal to read all bytes codes

// */
// }

int main(int argc, char** argv){
  if(argc != 2){
    fprintf(stderr, "ERROR: wrong arguments!\n");
    exit(1);
  }
  
  uni_vec_t* buffer = get_input_buffer(argv[1]);
  uni_vec_t* bf_arr = get_byte_freq_arr(buffer);

  // for(int i = 0; i < bf_arr->size; i++){
  //   byte_freq_t* bf = ((byte_freq_t*)uni_vec_get(bf_arr, i));
  //   printf("%c : %d\n", bf->byte, bf->freq);
  // }

  // huffman(bf_arr);

  uni_vec_free(buffer);
  // uni_vec_free(bf_arr); 
  return 0;
}

