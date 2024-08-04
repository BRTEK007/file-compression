#include <iostream>
#include <string.h>
#include <fstream>

#include "oddcod.hpp"

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

  std::ifstream inStream;
  std::ofstream outStream;

  inStream.open(input_filename, std::ifstream::binary);
  outStream.open(output_filename, std::ofstream::binary);

  if (!outStream.is_open())
  {
    fprintf(stderr, "ERROR: could not open file: %s\n", output_filename);
    exit(2);
  }
  if (!inStream.is_open())
  {
    fprintf(stderr, "ERROR: could not open file: %s\n", input_filename);
    exit(2);
  }

  bool wasError = false;

  if (doCompress)
  {
    try
    {
      oddcod::huffman::encode(inStream, outStream);
    }
    catch (std::exception &e)
    {
      std::cout << "ERROR: " << e.what() << '\n';
      wasError = true;
    }
  }
  else
  {
    oddcod::huffman::decode(inStream, outStream);
  }

  inStream.close();
  outStream.close();

  return wasError ? 1 : 0;
}
