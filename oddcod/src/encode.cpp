#include "oddcod.hpp"

#include "byte_freq.hpp"
#include "tree.hpp"
#include "bit_ostream.hpp"
#include "bit_code.hpp"
#include "bit_ostream.hpp"

#include <stdexcept>

namespace oddcod
{

  static Result huffman_encode(Input &bitIstream, Output &bitOstream)
  {
    std::vector<word_t> inBuffer;

    bitIstream.writeToVec(&inBuffer); // TODO don't write to vector for large files, just do 2 reads

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

  Result huffman::encode(std::istream &inStream, std::ostream &outStream)
  {
    StreamInput input(inStream);
    StreamOutput output(outStream);
    return huffman_encode(input, output);
  }
};