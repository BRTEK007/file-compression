#include <stdio.h>
#include <assert.h>

#include "byte_slice.h"
#include "bit_set.h"

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

    bit_set_free(&bit_set);
TEST_END

int main(int argc, char** argv){
  TEST_RUN(byte_slice_1);
  TEST_RUN(bit_set_1);
  return 0;
}

