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

bit_set_t compress(unsigned char* buffer){
  uint32_t total_byte_count = cvector_size(buffer);

  byte_freq_t* bf_arr = get_byte_freq_arr(buffer);

  unsigned char unique_byte_count = cvector_size(bf_arr);
  
  node_t* root = create_tree(bf_arr);
  
  byte_slice_t* codes = calloc(256, sizeof(byte_slice_t)); 
  
  tree_extract_codes(root, codes);

  printf("-------------------------\n");
  printf("COMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  printf("BYTE   | FREQUENCY | CODE\n");
  printf("-------------------------\n");
  for(int i = 0; i < unique_byte_count; i++){
    byte_freq_t bf = bf_arr[i];
    printf("%d (%c) | %9.1f | ", bf.byte, bf.byte, (float)(100*bf.freq) / total_byte_count);
    byte_slice_print(codes[bf.byte]);
    printf("\n");
  }
  printf("-------------------------\n");

  bit_set_t bit_set;
  bit_set_init(&bit_set);

  //write 4 bytes -> total bytes count
  unsigned char* bytes = (unsigned char *)&total_byte_count;
  bit_set_write_byte(&bit_set, bytes[0]);
  bit_set_write_byte(&bit_set, bytes[1]);
  bit_set_write_byte(&bit_set, bytes[2]);
  bit_set_write_byte(&bit_set, bytes[3]);
  //write 1 byte -> unique bytes count
  bit_set_write_byte(&bit_set, unique_byte_count);
  //write huffman tree data
  tree_write_to_bitset(root, &bit_set);
  //write compressed data
  for(uint32_t i = 0; i < total_byte_count; i++){
    unsigned char byte = buffer[i];
    byte_slice_t slice = codes[byte];
    bit_set_write_slice(&bit_set, slice);
  }
  bit_set_end_write(&bit_set);
  
  // decompress(&bit_set);
  //write bitset to file
  
  tree_free(root);
  free(codes);
  cvector_free(bf_arr);
  return bit_set;
}

void decompress(bit_set_t* bit_set){
  bit_set_begin_read(bit_set);
  //read 4 bytes -> total_characters count
  unsigned char bytes[4];
  bytes[0] = bit_set_read_byte(bit_set);
  bytes[1] = bit_set_read_byte(bit_set);
  bytes[2] = bit_set_read_byte(bit_set);
  bytes[3] = bit_set_read_byte(bit_set);
  uint32_t total_byte_count = *((uint32_t*)bytes);
  //read 1 byte -> unique bytes count
  unsigned char unique_byte_count = bit_set_read_byte(bit_set); 
  //
  printf("-------------------------\n");
  printf("DECOMPRESSING %d BYTES, %d UNIQUE\n", total_byte_count, unique_byte_count);
  printf("-------------------------\n");
  //read huffman tree data
  node_t* root = tree_read_from_bitset(bit_set, unique_byte_count);

  //
  cvector_vector_type(unsigned char) leaf_bytes = NULL;
  
  tree_extract_leaf_bytes(root, &leaf_bytes);

  byte_slice_t* codes = calloc(256, sizeof(byte_slice_t)); 
  
  tree_extract_codes(root, codes);

  printf("BYTE   | CODE\n");
  printf("-----------\n");
  for(int i = 0; i < cvector_size(leaf_bytes); i++){
    printf("%d (%c) | ", leaf_bytes[i], leaf_bytes[i]);
    byte_slice_print(codes[leaf_bytes[i]]);
    printf("\n");
  }
  printf("-------------------------\n");
  
  free(codes);
  cvector_free(leaf_bytes);

  //read compressed data
  cvector_vector_type(unsigned char) buffer = NULL;
  cvector_reserve(buffer, total_byte_count);
  uint32_t read_bytes = 0;
  node_t* node = root;
  while(read_bytes < total_byte_count){
    bool bit = bit_set_read_bit(bit_set);
    if(bit){
      node = node->right;
    }else{
      node = node->left;
    }
    if(node->leaf){
      // printf("%c", node->byte);
      cvector_push_back(buffer, node->byte);
      node = root;
      read_bytes++;
    }
  }

  //bytes to write are in buffer

  cvector_free(buffer);
  tree_free(root);
}

void args_error_exit(){
  fprintf(stderr, "ERROR: wrong arguments!, run with -h to display help.\n");
  exit(1);
}

void print_help(){
  printf(
    "----------------------------------\n"
    "./byte-compression [OPTION] [INPUT FILE] [OUTPUT FILE]\n"
    "OPTION e {-h, -c, -d}\n"
    "-h ... prints this message\n"
    "-c ... compress mode\n"
    "-d ... decompress mode\n"
    "INPUT FILE ... file to compress\n"
    "OUTPUT FILE ... file to write compressed result in\n"
  );
}

int main(int argc, char** argv){
  char* input_filename;
  char* output_filename;
  bool doCompress;

  if(argc > 1 && argc != 4){
    if(strcmp(argv[1], "-h") == 0){
      print_help();
      exit(0);
    }else
      args_error_exit();
  }else if(argc == 4){
    input_filename = argv[2];
    output_filename = argv[3];
    if(strcmp(argv[1], "-c") == 0){
      doCompress = true;
    }else if(strcmp(argv[1], "-d") == 0){
      doCompress = false;
    }else if(strcmp(argv[1], "-h") == 0){
      print_help();
      exit(0);
    }else{
      args_error_exit();
    }
  }else{
    args_error_exit();
  }
  
  unsigned char* buffer = get_input_buffer(input_filename);

  if(doCompress){
    bit_set_t bit_set = compress(buffer);
    
    FILE* outputFile;
  
    outputFile = fopen(output_filename, "wb");
    if(outputFile == NULL){
      fprintf(stderr, "ERROR: could not open file\n");
      exit(2);
    }

    fwrite(bit_set.bytes, sizeof(unsigned char), cvector_size(bit_set.bytes), outputFile);

    fclose(outputFile);

    bit_set_free(&bit_set);
    
    cvector_free(buffer);
  }else{
    //create bitset from buffer
    //the decompress it and return buffer to write to ouput file
  }

  
  return 0;
}
