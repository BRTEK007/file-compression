#ifndef IO_H
#define IO_H

#include <vector>
#include <istream>
void read_bytes_from_file(std::istream &fileStream, std::vector<unsigned char> &out_bytes);

#endif