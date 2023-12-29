#include "compress.hpp"

#include "byte_freq.hpp"
#include "tree.hpp"
#include "bit_ostream.hpp"
#include "bit_code.hpp"
#include "bit_ostream.hpp"

#include <stdexcept>

inline void streamToVector(std::istream &inStream, std::vector<unsigned char> &outVector)
{
  inStream.seekg(0, std::ios::end);
  auto streamSize = inStream.tellg();
  inStream.seekg(0, std::ios::beg);

  outVector.clear();
  outVector.resize(streamSize);
  inStream.read(reinterpret_cast<char *>(outVector.data()), streamSize);
}

void compress(std::istream &inStream, std::ostream &outStream)
{
  std::vector<unsigned char> inBuffer;
  streamToVector(inStream, inBuffer);

  if (inBuffer.size() == 0)
  {
    throw std::runtime_error("Can't compress 0B file.");
  }

  auto byteFreqArr = findByteFrequencies(inBuffer);

  auto tree = Tree(byteFreqArr);

  std::array<BitCode, 256> codes;
  tree.extractCodes(codes);

  auto bitOstream = BitOstream(outStream);

  //  write 2 bytes -> unique bytes count
  uint16_t uniqueByteCount = byteFreqArr.size();
  unsigned char *bytes = reinterpret_cast<unsigned char *>(&uniqueByteCount);
  bitOstream.write(bytes[0]);
  bitOstream.write(bytes[1]);
  // write huffman tree data
  tree.writeTo(bitOstream);
  // write compressed data
  for (auto byte : inBuffer)
  {
    BitCode code = codes[byte];
    bitOstream.write(code);
  }
  bitOstream.flush();
}