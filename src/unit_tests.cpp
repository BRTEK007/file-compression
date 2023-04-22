#include <stdio.h>
#include <assert.h>

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
    bit_set_t bit_set;
    bit_set_init(&bit_set);

    //11001101 1011
    bit_set_write_bit(&bit_set, true);
    bit_set_write_bit(&bit_set, true);
    bit_set_write_bit(&bit_set, false);
    bit_set_write_bit(&bit_set, false);
    
    bit_set_write_bit(&bit_set, true);
    bit_set_write_bit(&bit_set, true);
    bit_set_write_bit(&bit_set, false);
    bit_set_write_bit(&bit_set, true);
    
    bit_set_write_bit(&bit_set, true);
    bit_set_write_bit(&bit_set, false);
    bit_set_write_bit(&bit_set, true);
    bit_set_write_bit(&bit_set, true);

    bit_set_end_write(&bit_set);
    
    bit_set_begin_read(&bit_set);
    //11001101 1011
    assert(bit_set_read_bit(&bit_set) == true);
    assert(bit_set_read_bit(&bit_set) == true);
    assert(bit_set_read_bit(&bit_set) == false);
    assert(bit_set_read_bit(&bit_set) == false);
    
    assert(bit_set_read_bit(&bit_set) == true);
    assert(bit_set_read_bit(&bit_set) == true);
    assert(bit_set_read_bit(&bit_set) == false);
    assert(bit_set_read_bit(&bit_set) == true);
   
    assert(bit_set_read_bit(&bit_set) == true);
    assert(bit_set_read_bit(&bit_set) == false);
    assert(bit_set_read_bit(&bit_set) == true);
    assert(bit_set_read_bit(&bit_set) == true);
   
    bit_set_free(&bit_set);
TEST_END

TEST_BEGIN(bit_set_2)
    bit_set_t bit_set;
    bit_set_init(&bit_set);

    // 0 0 1 A 0 1 F 
    bit_set_write_bit(&bit_set, false);
    bit_set_write_bit(&bit_set, false);
    bit_set_write_bit(&bit_set, true);

    bit_set_write_byte(&bit_set, 'A');
    
    bit_set_write_bit(&bit_set, false);
    bit_set_write_bit(&bit_set, true);

    bit_set_write_byte(&bit_set, 'F');
    
    bit_set_end_write(&bit_set);
    
    bit_set_begin_read(&bit_set);
    // 0 0 1 A 0 1 F 
    assert(bit_set_read_bit(&bit_set) == false);
    assert(bit_set_read_bit(&bit_set) == false);
    assert(bit_set_read_bit(&bit_set) == true);
    
    assert(bit_set_read_byte(&bit_set) == 'A');
    
    assert(bit_set_read_bit(&bit_set) == false);
    assert(bit_set_read_bit(&bit_set) == true);

    assert(bit_set_read_byte(&bit_set) == 'F');
    bit_set_free(&bit_set);
TEST_END

int main(int argc, char** argv){
  TEST_RUN(byte_slice_1);
  TEST_RUN(bit_set_1);
  TEST_RUN(bit_set_2);
  return 0;
}

