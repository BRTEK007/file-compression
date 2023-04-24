#include <stdio.h>
#include <assert.h>
#include <iostream>

#include "byte_slice.hpp"
#include "bit_set.hpp"
#include "byte_freq.hpp"
#include "tree.hpp"

#define TEST_BEGIN(name) void name() { \
                        printf("RUNNING TEST\n");
#define TEST_END printf("SUCCESS\n");}
#define TEST_RUN(name) name();

TEST_BEGIN(byte_slice_1)
    ByteSlice byte_slice;

    //1101 
    byte_slice.write_bit(true);
    byte_slice.write_bit(true);
    byte_slice.write_bit(false);
    byte_slice.write_bit(true);

    //1101 = 13
    assert(byte_slice.bits == (unsigned char)13);

    //1101
    assert(byte_slice.read_bit() == true);
    assert(byte_slice.read_bit() == true);
    assert(byte_slice.read_bit() == false);
    assert(byte_slice.read_bit() == true);

TEST_END


TEST_BEGIN(bit_set_1)
    BitSet bit_set;

    //11001101 1011
    bit_set.write_bit(true);
    bit_set.write_bit(true);
    bit_set.write_bit(false);
    bit_set.write_bit(false);
    
    bit_set.write_bit(true);
    bit_set.write_bit(true);
    bit_set.write_bit(false);
    bit_set.write_bit(true);
    
    bit_set.write_bit(true);
    bit_set.write_bit(false);
    bit_set.write_bit(true);
    bit_set.write_bit(true);

    bit_set.end_write();
    
    bit_set.begin_read();
    //11001101 1011
    assert(bit_set.read_bit() == true);
    assert(bit_set.read_bit() == true);
    assert(bit_set.read_bit() == false);
    assert(bit_set.read_bit() == false);
    
    assert(bit_set.read_bit() == true);
    assert(bit_set.read_bit() == true);
    assert(bit_set.read_bit() == false);
    assert(bit_set.read_bit() == true);
   
    assert(bit_set.read_bit() == true);
    assert(bit_set.read_bit() == false);
    assert(bit_set.read_bit() == true);
    assert(bit_set.read_bit() == true);
   
TEST_END

TEST_BEGIN(bit_set_2)
    BitSet bit_set;

    // 0 0 1 A 0 1 F 
    bit_set.write_bit(false);
    bit_set.write_bit(false);
    bit_set.write_bit(true);

    bit_set.write_byte('A');
    
    bit_set.write_bit(false);
    bit_set.write_bit(true);

    bit_set.write_byte('F');
    
    bit_set.end_write();
    
    bit_set.begin_read();
    // 0 0 1 A 0 1 F 
    assert(bit_set.read_bit() == false);
    assert(bit_set.read_bit() == false);
    assert(bit_set.read_bit() == true);
    
    assert(bit_set.read_byte() == 'A');
    
    assert(bit_set.read_bit() == false);
    assert(bit_set.read_bit() == true);

    assert(bit_set.read_byte() == 'F');
TEST_END

TEST_BEGIN(bit_code_1)
    BitCode code;
    code.writeBit(true);
    code.writeBit(true);
    code.writeBit(false);
    code.writeBit(true);
    //1101
    assert(code.readBit() == true);
    assert(code.readBit() == true);
    assert(code.readBit() == false);
    assert(code.readBit() == true);
TEST_END

TEST_BEGIN(bit_code_2)
    BitCode code;
    
    code.writeBit(true);
    code.writeBit(true);
    code.writeBit(false);
    code.writeBit(true);
    code.writeBit(true);
    code.writeBit(false);
    code.writeBit(true);
    code.writeBit(false);
    //11011010 = 218
    assert(code.readByte() == 218);
TEST_END

TEST_BEGIN(all_bytes_1)
    std::vector<byte_freq_t> bf_arr;
    for(int i = 0; i < 256; i++){
        byte_freq_t bf;
        bf.byte = i;
        bf.freq = 1;
        bf_arr.push_back(bf);
    }

    Tree tree;
    tree.create_from_bytefreq(bf_arr);

    std::array<BitCode, 256> codes; 

    tree.extract_codes(codes);

    for(int i = 0; i < 256; i++){
        assert(codes[i].size() > 0);
    }

TEST_END

int main(int argc, char** argv){
  TEST_RUN(byte_slice_1);
  TEST_RUN(bit_set_1);
  TEST_RUN(bit_set_2);
  TEST_RUN(bit_code_1);
  TEST_RUN(bit_code_2);
  TEST_RUN(all_bytes_1);
  return 0;
}

