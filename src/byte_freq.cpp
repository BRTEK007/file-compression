#include "byte_freq.hpp"
#include <stdlib.h>
// #include "c-vector/cvector.h"

int cmp_byte_freq (const void * a, const void * b) {
   return ( ((byte_freq_t*)a)->freq - ((byte_freq_t*)b)->freq );
}

//returns array of byte_frequencies from input byte buffer
std::vector<byte_freq_t> get_byte_freq_arr(std::vector<unsigned char> input){ 
  byte_freq_t* arr = calloc(256, sizeof(byte_freq_t));//256 bytes, count their frequency
  
  for(int i = 0; i < 256; i++){//init array
    arr[i].byte = (unsigned char)i;
    arr[i].freq = 0;
  }

  for(int i = 0; i < input.size(); i++){//go through bytes in input file and increment their freq in array
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

  std::vector<byte_freq_t> bf_arr;
  bf_arr.reserve(size);

  //copy bytes with atleast 1 occurance to new array
  for(int i = 0; i < size; i++){
    // cvector_push_back(bf_arr, arr[zeros+i]);
    bf_arr.push_back(arr[zeros+i]);
  }

  free(arr);
  
  return bf_arr;
}