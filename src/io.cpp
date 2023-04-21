#include "io.hpp"
#include <stdio.h>
// #include "c-vector/cvector.h"
#include <vector>
#include <stdlib.h>

//reads bytes from file, returns them in a cvector
std::vector<unsigned char> read_bytes_from_file(const char* filename){ 
  FILE* inputFile;
  
  inputFile = fopen(filename, "rb");
  if(inputFile == NULL){
    fprintf(stderr, "ERROR: could not open file\n");
    exit(2);
  }
  
  fseek(inputFile, 0, SEEK_END);        // Jump to the end of the file
  long filelen = ftell(inputFile);      // Get the current byte offset in the file
  rewind(inputFile);                      // Jump back to the beginning of the file

  unsigned char* buffer = (unsigned char*) calloc(filelen, sizeof(unsigned char)); // Enough memory for the file
  fread(buffer, filelen, 1, inputFile); // Read in the entire file 

  fclose(inputFile); // Close the file

  // cvector_vector_type(unsigned char) vec = NULL;

  // cvector_reserve(vec, filelen);

  std::vector<unsigned char> vec;
  vec.reserve(filelen);

  for(long i = 0; i < filelen; i++){
    vec.push_back(buffer[i]);
    //cvector_push_back(vec, buffer[i]);
  }

  free(buffer);
  return vec;
}