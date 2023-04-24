#ifndef BIT_SET_H
#define BIT_SET_H

#include "bit_code.hpp"
#include "byte_slice.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <vector>

//end writing with end_write()
//start reading with begin_read()
//WRITE: first write to byte_slice if full push its bits to bytes
//READ: copy n-th byte to byte_slice, read from byte_slice, if empty -> read copy another byte

class BitSet{//TODO optimze io operations
   private:
      std::vector<unsigned char> bytes;
      ByteSlice byte_slice;//byte slice to write or to read, TODO replace with BitCode
      uint64_t read_id;//id of byte to read
      bool owning_bytes;//TODO reimplement this
   public:
      BitSet();
      ~BitSet();
      void create_from_bytes(std::vector<unsigned char> bytes);
      void end_write();
      void begin_read();
      void write_bit(bool bit);
      void write_byte(unsigned char byte);
      void write_slice(ByteSlice slice);
      void write_bitcode(BitCode bc);
      bool read_bit();
      unsigned char read_byte();
      std::vector<unsigned char> extract_bytes();//TODO rewrite to not copy
};

#endif