#include "oddcod.hpp"

#include "byte_freq.hpp"
#include "tree.hpp"
#include "bit_ostream.hpp"
#include "bit_code.hpp"
#include "bit_ostream.hpp"

#include <stdexcept>

static oddcod::Result huffman_encode(const std::vector<oddcod::word_t> &inBuffer, BitOstream &bitOstream)
{
  if (inBuffer.size() == 0)
  {
    throw std::runtime_error("Can't compress 0B file.");
  }

  auto byteFreqArr = findByteFrequencies(inBuffer);

  auto tree = Tree(byteFreqArr);

  std::array<BitCode, 256> codes;
  tree.extractCodes(codes);

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

  return oddcod::Result::OK;
}
namespace oddcod
{

  // Result huffman::encode(const std::vector<word_t> &inBuffer, std::vector<word_t> &outBuffer)
  //{
  // VectorByteWritable vectorByteWritable(outBuffer);
  // auto bitOstream = BitOstream(vectorByteWritable);
  // return huffman::encode(inBuffer, bitOstream);
  //}

  template <typename T>
  Result huffman::decode(Input<T> input, Output<T> output);
  {
    std::vector<word_t> inBuffer;
    streamToVector(inStream, inBuffer);
    StreamByteWritable streamByteWritable(outStream);
    auto bitOstream = BitOstream(streamByteWritable);
    return huffman_encode(inBuffer, bitOstream);
  }
};