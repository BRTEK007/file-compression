#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fstream>

#include "compress.hpp"
#include "decompress.hpp"
#include "io.hpp"

#define CVECTOR_LOGARITHMIC_GROWTH

void args_error_exit()
{
  fprintf(stderr, "ERROR: wrong arguments!, run with -h to display help.\n");
  exit(1);
}

void print_help()
{
  printf(
      "----------------------------------\n"
      "./byte-compression [OPTION] [INPUT FILE] [OUTPUT FILE]\n"
      "OPTION e {-h, -c, -d}\n"
      "-h ... prints this message\n"
      "-c ... compress mode\n"
      "-d ... decompress mode\n"
      "INPUT FILE ... file to compress\n"
      "OUTPUT FILE ... file to write compressed result in\n");
}

int main(int argc, char **argv)
{
  char *input_filename;
  char *output_filename;
  bool doCompress;

  // TODO better args
  if (argc > 1 && argc != 4)
  {
    if (strcmp(argv[1], "-h") == 0)
    {
      print_help();
      exit(0);
    }
    else
      args_error_exit();
  }
  else if (argc == 4)
  {
    input_filename = argv[2];
    output_filename = argv[3];
    if (strcmp(argv[1], "-c") == 0)
    {
      doCompress = true;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
      doCompress = false;
    }
    else if (strcmp(argv[1], "-h") == 0)
    {
      print_help();
      exit(0);
    }
    else
    {
      args_error_exit();
    }
  }
  else
  {
    args_error_exit();
  }

  std::vector<unsigned char> input_buffer;
  read_bytes_from_file(input_filename, input_buffer);

  std::ofstream outStream;
  outStream.open(output_filename, std::ifstream::binary);

  if (!outStream.is_open())
  {
    fprintf(stderr, "ERROR: could not open file: %s\n", output_filename);
    exit(2);
  }

  if (doCompress)
  {
    compress(input_buffer, outStream);
  }
  else
  {
    decompress(input_buffer, outStream);
  }

  outStream.close();

  return 0;
}
