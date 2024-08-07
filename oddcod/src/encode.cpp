#include "oddcod.hpp"

#include "byte_freq.hpp"
#include "tree.hpp"
#include "bit_ostream.hpp"
#include "bit_code.hpp"
#include "bit_ostream.hpp"

#include <stdexcept>

namespace oddcod
{
  static Result huffman_encode(std::shared_ptr<AlignedReader> bitIstream, Writer &bitOstream)
  {
    if (bitIstream->eof())
    {
      return Result::ERR_ENCODE_EMPTY;
    }

    auto byteFreqArr = findByteFrequencies(bitIstream);

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
    bitIstream->resetToBegin();
    while (!bitIstream->eof())
    {
      auto byte = bitIstream->readWord();
      BitCode code = codes[byte];
      bitOstream.write(code);
    }
    bitOstream.flush();

    return oddcod::Result::OK;
  }

  Result huffman::encode(std::istream &inStream, std::ostream &outStream)
  {
    // StreamBitReader input(inStream);
    ReaderInput input(&inStream);
    auto reader = input.createAlignedReader();
    StreamWriter output(outStream);
    return huffman_encode(reader, output);
  }
};