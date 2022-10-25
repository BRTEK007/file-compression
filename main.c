#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
  if(argc != 2){
    fprintf(stderr, "ERROR: wrong arguments!\n");
    exit(1);
  }
  
  FILE* inputFile;
  
  inputFile = fopen(argv[1], "rb");
  if(inputFile == NULL){
    fprintf(stderr, "ERROR: could not open file\n");
    exit(2);
  }

  
  fseek(inputFile, 0, SEEK_END);        // Jump to the end of the file
  size_t filelen = ftell(inputFile);      // Get the current byte offset in the file
  rewind(inputFile);                      // Jump back to the beginning of the file

  unsigned char* buffer = calloc(filelen, sizeof(unsigned char)); // Enough memory for the file
  fread(buffer, filelen, 1, inputFile); // Read in the entire file

  int array[256];//holds how many each byte appeared in the file
  
  memset(array, 0, 256*sizeof(int));

  for(size_t i = 0; i < filelen; i++){
    array[buffer[i]]++;
  }

  for(int i = 0; i < 256; i++){
    printf("%d : %d\n", i, array[i]);
  }

  printf("total bytes: %ld\n", filelen);

  free(buffer);
  fclose(inputFile); // Close the file
  return 0;
}
