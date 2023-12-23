#include "io.hpp"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <fstream>

//reads bytes from file, returns them in a cvector
void read_bytes_from_file(const char* filename, std::vector<unsigned char>& out_bytes){ 
  std::ifstream fileStream;
  fileStream.open(filename, std::ifstream::binary);
  if(!fileStream.is_open()){
    fprintf(stderr, "ERROR: could not open file\n");
    exit(2);
  }
  
  auto pbuf = fileStream.rdbuf();

  auto size = pbuf->pubseekoff (0,fileStream.end,fileStream.in);
  pbuf->pubseekpos (0,fileStream.in);

  char* buffer=new char[size];

  pbuf->sgetn (buffer,size);
  
  out_bytes.clear();
  out_bytes.reserve(size);

  for(long i = 0; i < size; i++){
    out_bytes.push_back(buffer[i]);
  }

  delete[] buffer;

  fileStream.close();
}