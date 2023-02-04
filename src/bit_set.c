#include "bit_set.h"

#include "c-vector/cvector.h"
// #include <stdio.h>

void bit_set_init(bit_set_t* bit_set){
    cvector_vector_type(unsigned char) bytes = NULL;
    byte_slice_init(&(bit_set->byte_slice));
    bit_set->bytes = bytes;
    bit_set->read_id = 0;
}

void bit_set_end_write(bit_set_t* bit_set){
    //push byte_slice to bytes, bit by bit
    unsigned char byte = bit_set->byte_slice.bits;
    byte = byte << (8-bit_set->byte_slice.len);
    
    cvector_push_back(bit_set->bytes, byte);
}

void bit_set_begin_read(bit_set_t* bit_set){
    byte_slice_t* slice = &(bit_set->byte_slice);
    bit_set->read_id = 0;
    slice->bits = bit_set->bytes[bit_set->read_id];
    slice->len = 8;
    bit_set->read_id++;
}

void bit_set_free(bit_set_t* bit_set){
    cvector_free(bit_set->bytes);
}

void bit_set_write_bit(bit_set_t* bit_set, bool bit){
    byte_slice_t* slice = &(bit_set->byte_slice);
    byte_slice_write_bit(slice, bit);
    if(slice->len == 8){//is full, can be added to bytes
        cvector_push_back(bit_set->bytes, slice->bits);
        byte_slice_init(slice);
    }
}

bool bit_set_read_bit(bit_set_t* bit_set){
    byte_slice_t* slice = &(bit_set->byte_slice);

    bool bit = byte_slice_read_bit(slice);

    if(slice->len == 0){//slice is empty -> copy next byte to slice
        slice->bits = bit_set->bytes[bit_set->read_id];
        slice->len = 8;
        bit_set->read_id++;
    }

    return bit;
}