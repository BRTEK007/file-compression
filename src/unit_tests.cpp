#include <stdio.h>
#include <assert.h>
#include <iostream>

#include "byte_slice.hpp"
#include "bit_set.hpp"

#define TEST_BEGIN(name) void name() { \
                        printf("RUNNING TEST\n");
#define TEST_END printf("SUCCESS\n");}
#define TEST_RUN(name) name();

TEST_BEGIN(byte_slice_1)
    byte_slice_t byte_slice;
    byte_slice_init(&byte_slice);

    //1101 
    byte_slice_write_bit(&byte_slice, true);
    byte_slice_write_bit(&byte_slice, true);
    byte_slice_write_bit(&byte_slice, false);
    byte_slice_write_bit(&byte_slice, true);

    //1101 = 13
    assert(byte_slice.bits == (unsigned char)13);

    //1101
    assert(byte_slice_read_bit(&byte_slice) == true);
    assert(byte_slice_read_bit(&byte_slice) == true);
    assert(byte_slice_read_bit(&byte_slice) == false);
    assert(byte_slice_read_bit(&byte_slice) == true);

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

int main(int argc, char** argv){
  TEST_RUN(byte_slice_1);
  TEST_RUN(bit_set_1);
  TEST_RUN(bit_set_2);
  TEST_RUN(bit_code_1);
  TEST_RUN(bit_code_2);
  return 0;
}

