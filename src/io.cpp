#include "io.hpp"
#include <stdio.h>
#include <vector>
#include <stdlib.h>

//reads bytes from file, returns them in a cvector
void read_bytes_from_file(const char* filename, std::vector<unsigned char>& out_bytes){ 
  FILE* inputFile;
  
  inputFile = fopen(filename, "rb");
  if(inputFile == NULL){
    fprintf(stderr, "ERROR: could not open file\n");
    exit(2);
  }
  
  fseek(inputFile, 0, SEEK_END);        // Jump to the end of the file
  long filelen = ftell(inputFile);      // Get the current byte offset in the file
  rewind(inputFile);                      // Jump back to the beginning of the file

  unsigned char* buffer = new unsigned char[filelen]; // Enough memory for the file
  fread(buffer, filelen, 1, inputFile); // Read in the entire file 

  fclose(inputFile); // Close the file

  out_bytes.clear();
  out_bytes.reserve(filelen);

  for(long i = 0; i < filelen; i++){
    out_bytes.push_back(buffer[i]);
  }

  delete buffer;
}