#ifndef BIT_SET_H
#define BIT_SET_H

#include "byte_slice.hpp"
#include "bit_code.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <vector>

//end writing with end_write()
//start reading with begin_read()
//WRITE: first write to byte_slice if full push its bits to bytes
//READ: copy n-th byte to byte_slice, read from byte_slice, if empty -> read copy another byte

// typedef struct bit_set_t{
//    std::vector<unsigned char> bytes;
//    byte_slice_t byte_slice;//byte slice to write or to read
//    uint64_t read_id;//id of byte to read
//    bool owning_bytes;
// } bit_set_t;

// void bit_set_init(bit_set_t* bit_set);

// void bit_set_init_from_bytes(bit_set_t* bit_set, std::vector<unsigned char> bytes);

// void bit_set_end_write(bit_set_t* bit_set);

// void bit_set_begin_read(bit_set_t* bit_set);

// void bit_set_free(bit_set_t* bit_set);

// void bit_set_write_bit(bit_set_t* bit_set, bool bit);

// bool bit_set_read_bit(bit_set_t* bit_set);

// void bit_set_write_byte(bit_set_t* bit_set, unsigned char byte);

// unsigned char bit_set_read_byte(bit_set_t* bit_set);

// void bit_set_write_slice(bit_set_t* bit_set, byte_slice_t slice);

// std::vector<unsigned char> bit_set_extract_bytes(bit_set_t* bit_set);

// void bit_set_write_code(bit_set_t* bit_set, BitCode bc);

class BitSet{//TODO optimze io operations
   private:
      std::vector<unsigned char> bytes;
      byte_slice_t byte_slice;//byte slice to write or to read, TODO replace with BitCode
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
      void write_slice(byte_slice_t slice);
      void write_bitcode(BitCode bc);
      bool read_bit();
      unsigned char read_byte();
      std::vector<unsigned char> extract_bytes();//TODO rewrite to not copy
};

#endif