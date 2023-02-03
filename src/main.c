#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bitcode.h"
#include "node_queue.h"
#include "tree.h"

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
  
  bit_code_t* bit_codes = calloc(256, sizeof(bit_code_t));
  
  bit_code_t bc = bit_code_empty();
  
  tree_traversal(root, bc, bit_codes);
  tree_traversal_recon(root);

  // unsigned char byte;
  // bit_code_t bits = bit_code_empty();

  // add_bits(&bits, );
  // bit_code_t to_write = bit_code_empty();

  // for(int i = 0; i < cvector_size(buffer); i++){
  //   unsigned char byte = buffer[i];
  //   bit_code_t bits = bit_codes[byte];
  //   // printf("(");
  //   // bit_code_print(bits);
  //   // printf(")");
  //   while(bits.len > 0){
  //     int bit = bit_code_pop_msb(&bits);
  //     // printf("(%d)", bit);
  //     if(bit)
  //       to_write = bit_code_add_one(to_write);
  //     else
  //       to_write = bit_code_add_zero(to_write);
  //     if(to_write.len == 8){
  //       bit_code_print(to_write);
  //       printf(" ");
  //       to_write = bit_code_empty();
  //     }
  //     // bit_code_print(to_write);
  //     // printf("\n");
  //   }

  // }
  //remember about the rest int to_write 
  // bit_code_print(to_write);
  
  tree_free(root);

  free(bit_codes);
  cvector_free(buffer);
  cvector_free(bf_arr);
  return 0;
}

