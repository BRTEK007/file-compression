#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "compress.hpp"
#include "decompress.hpp"
#include "io.hpp"

#define CVECTOR_LOGARITHMIC_GROWTH
// #include "c-vector/cvector.h"

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
  // printf("1\n");
  
  unsigned char* input_buffer = read_bytes_from_file(input_filename);
  unsigned char* output_buffer;

  // printf("2\n");
  if(doCompress){
    output_buffer = compress(input_buffer);
  }else{
    // printf("3\n");
    output_buffer = decompress(input_buffer);
  }
  // printf("4\n");

  FILE* output_file;
  
  output_file = fopen(output_filename, "wb");
  if(output_file == NULL){
    fprintf(stderr, "ERROR: could not open file: %s\n", output_filename);
    exit(2);
  }

  fwrite(output_buffer, sizeof(unsigned char), cvector_size(output_buffer), output_file);

  cvector_free(input_buffer);
  cvector_free(output_buffer);
  fclose(output_file);

  return 0;
}
