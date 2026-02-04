#include "oddcod.hpp"

#include "tree.hpp"
#include "bit_istream.hpp"
namespace oddcod
{
  static Result huffman_decode(std::shared_ptr<BitReader> bitIstream, std::shared_ptr<AlignedWriter> outStream)
  {
    // read 2 bytes -> unique bytes count
    unsigned char bytes[2];
    bytes[0] = bitIstream->readByte(); // TODO possible error/ might file be too small
    bytes[1] = bitIstream->readByte();
    uint16_t uniqueByteCount = *(reinterpret_cast<uint16_t *>(bytes));

    CodeTree tree;
    tree.readFrom(bitIstream, uniqueByteCount); // TODO possible error

    std::vector<unsigned char> leafBytes;
    tree.extractLeafBytes(leafBytes);

    std::array<BitCode, 256> codes;
    tree.extractCodes(codes);

    tree.ptrReset();
    while (!bitIstream->eof())
    {
      bool bit = bitIstream->readBit();

      if (bit)
        tree.ptrRight();
      else
        tree.ptrLeft();

      if (tree.ptrIsNull()){
        return Result::ERR_DECODE_TREE;
      }

      if (tree.ptrIsLeaf())
      {
        auto byte = tree.ptrReadByte();
        outStream->write(&byte, 1);
        tree.ptrReset();
      }
    }
    return Result::OK;
  }
  Result huffman::decode(std::istream &inStream, std::ostream &outStream)
  {
    StreamReaderProvider input(&inStream);
    StreamWriterProvider output(&outStream);

    auto reader = input.createBitReader();
    if (!reader)
      return Result::ERR_INPUT;
    auto writer = output.createAlignedWriter();
    if (!writer)
      return Result::ERR_OUTPUT;
      
    return huffman_decode(reader, writer);
  }
};