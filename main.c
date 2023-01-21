#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct byte_freq_t{
  unsigned char byte;
  int freq;
} byte_freq_t;

typedef struct byte_freq_arr_t{
  byte_freq_t* arr;
  size_t size;
}byte_freq_arr_t;

typedef struct byte_buffer_t{
  unsigned char* arr;
  size_t size;
}byte_buffer_t;

int cmp_byte_freq (const void * a, const void * b) {
   return ( ((byte_freq_t*)a)->freq - ((byte_freq_t*)b)->freq );
}

byte_freq_arr_t* get_byte_freq_arr(byte_buffer_t* input){
  byte_freq_arr_t* bf_arr = malloc(sizeof(byte_freq_arr_t));
  
  byte_freq_t* arr = calloc(256, sizeof(byte_freq_t));//256 bytes, count their frequency
  
  for(int i = 0; i < 256; i++){//init array
    arr[i].byte = (unsigned char)i;
    arr[i].freq = 0;
  }

  for(size_t i = 0; i < input->size; i++){//go through bytes in input file and increment their freq in array
    arr[input->arr[i]].freq++;
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

  //copy bytes with atleast 1 occurance to new array
  byte_freq_t* cropped_arr = calloc(size, sizeof(byte_freq_t));
  memcpy(cropped_arr, arr + zeros, size * sizeof(byte_freq_t));

  free(arr);

  bf_arr->arr = cropped_arr;
  bf_arr->size = size;
  return bf_arr;
}

byte_buffer_t* get_input_buffer(const char* filename){
  byte_buffer_t* bb = malloc(sizeof(byte_buffer_t));
  
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

  bb->size = filelen;
  bb->arr = buffer;
  return bb;
}

void byte_buffer_free(byte_buffer_t* bb){
  free(bb->arr);
  free(bb);
}

void byte_freq_arr_free(byte_freq_arr_t* bf_arr){
  free(bf_arr->arr);
  free(bf_arr);
}

int main(int argc, char** argv){
  if(argc != 2){
    fprintf(stderr, "ERROR: wrong arguments!\n");
    exit(1);
  }
  
  byte_buffer_t* buffer = get_input_buffer(argv[1]);
  byte_freq_arr_t* bf_arr = get_byte_freq_arr(buffer);

  for(int i = 0; i < bf_arr->size; i++){
    printf("%c : %d\n", bf_arr->arr[i].byte, bf_arr->arr[i].freq);
  }

  // printf("total bytes: %ld\n", buffer->size);
  
  byte_buffer_free(buffer);
  byte_freq_arr_free(bf_arr);
  return 0;
}
