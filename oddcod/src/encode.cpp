#include "oddcod.hpp"

#include "byte_freq.hpp"
#include "tree.hpp"
#include "bit_ostream.hpp"
#include "bit_code.hpp"
#include "bit_ostream.hpp"

#include <stdexcept>

namespace oddcod
{
  static Result huffman_encode(std::shared_ptr<AlignedReader> bitIstream, std::shared_ptr<BitWriter> bitOstream)
  {
    if (bitIstream->eof())
    {
      return Result::ERR_ENCODE_EMPTY;
    }

    auto byteFreqArr = findByteFrequencies(bitIstream);

    auto tree = CodeTree(byteFreqArr);

    std::array<BitCode, 256> codes;
    tree.extractCodes(codes);

    //  write 2 bytes -> unique bytes count
    uint16_t uniqueByteCount = byteFreqArr.size();
    unsigned char *bytes = reinterpret_cast<unsigned char *>(&uniqueByteCount);
    bitOstream->write(bytes[0]);
    bitOstream->write(bytes[1]);
    // write huffman tree data
    tree.writeTo(bitOstream);
    // write compressed data
    bitIstream->resetToBegin();
    while (!bitIstream->eof())
    {
      auto byte = bitIstream->readWord();
      BitCode code = codes[byte];
      bitOstream->write(code);
    }
    bitOstream->flush();

    return oddcod::Result::OK;
  }

  Result huffman::encode(std::istream &inStream, std::ostream &outStream)
  {
    StreamReaderProvider input(&inStream);
    StreamWriterProvider output(&outStream);

    auto reader = input.createAlignedReader();
    if (!reader)
      return Result::ERR_INPUT;
    auto writer = output.createBitWriter();
    if (!writer)
      return Result::ERR_OUTPUT;

    return huffman_encode(reader, writer);
  }
};