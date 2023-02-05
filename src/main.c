#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "byte_slice.h"
#include "node_queue.h"
#include "tree.h"
#include "bit_set.h"

#define CVECTOR_LOGARITHMIC_GROWTH
#include "c-vector/cvector.h"

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

  //sort array, count zeros (bytes that didnt exist in file)
  qsort(arr, 256, sizeof(byte_freq_t), cmp_byte_freq);

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
  long filelen = ftell(inputFile);      // Get the current byte offset in the file
  rewind(inputFile);                      // Jump back to the beginning of the file

  unsigned char* buffer = calloc(filelen, sizeof(unsigned char)); // Enough memory for the file
  fread(buffer, filelen, 1, inputFile); // Read in the entire file 

  fclose(inputFile); // Close the file

  cvector_vector_type(unsigned char) vec = NULL;

  cvector_reserve(vec, filelen);

  for(long i = 0; i < filelen; i++){
    cvector_push_back(vec, buffer[i]);
  }

  free(buffer);
  return vec;
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

  node_t* root = create_tree(bf_arr);
  
  byte_slice_t* codes = calloc(256, sizeof(byte_slice_t)); 
  
  tree_extract_codes(root, codes);

  // for(int i = 0; i < cvector_size(bf_arr); i++){
  //   byte_freq_t bf = bf_arr[i];
  //   printf("%c :", bf.byte);
  //   byte_slice_print(codes[bf.byte]);
  //   printf("\n");
  // }

  // tree_traversal_recon(root);

  // bitstack_t bitstack;
  // bitstack_init(&bitstack);

  // for(int i = 0; i < cvector_size(buffer); i++){
  //   unsigned char byte = buffer[i];
  //   bit_code_t bits = bit_codes[byte];
  //   bitstack_push_bits(&bitstack, bits);
  // }
  // bitstack_print(&bitstack);

  // bitstack_free(&bitstack);

  tree_free(root);

  // free(bit_codes);
  cvector_free(buffer);
  cvector_free(bf_arr);
  return 0;
}

