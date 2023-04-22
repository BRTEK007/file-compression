#include "bit_set.hpp"

#include <vector>
#include <stdio.h>

void bit_set_init(bit_set_t* bit_set){
    // cvector_vector_type(unsigned char) bytes = NULL;
    // printf("2 ");
    byte_slice_init(&(bit_set->byte_slice));
    // bit_set->bytes = bytes;
    bit_set->read_id = 0;
    bit_set->owning_bytes = true;
}

void bit_set_init_from_bytes(bit_set_t* bit_set, std::vector<unsigned char> bytes){
    byte_slice_init(&(bit_set->byte_slice));
    bit_set->bytes = bytes;
    bit_set->read_id = 0;
    bit_set->owning_bytes = false;
}

void bit_set_end_write(bit_set_t* bit_set){
    //push byte_slice to bytes, bit by bit
    unsigned char byte = bit_set->byte_slice.bits;
    byte = byte << (BYTE_SLICE_BIT_COUNT-bit_set->byte_slice.len);
    
    // cvector_push_back(bit_set->bytes, byte);
    bit_set->bytes.push_back(byte);
}

void bit_set_begin_read(bit_set_t* bit_set){
    byte_slice_t* slice = &(bit_set->byte_slice);
    bit_set->read_id = 0;
    slice->bits = bit_set->bytes[bit_set->read_id];
    slice->len = BYTE_SLICE_BIT_COUNT;
    bit_set->read_id++;
}

void bit_set_free(bit_set_t* bit_set){
    if(bit_set->owning_bytes){
        // cvector_free(bit_set->bytes);
    }
}

void bit_set_write_bit(bit_set_t* bit_set, bool bit){
    //write bit to slice, if slice full write slice to bytes
    byte_slice_t* slice = &(bit_set->byte_slice);
    byte_slice_write_bit(slice, bit);
    if(slice->len == BYTE_SLICE_BIT_COUNT){//is full, can be added to bytes
        // cvector_push_back(bit_set->bytes, slice->bits);
        bit_set->bytes.push_back(slice->bits);
        byte_slice_init(slice);
    }
}

bool bit_set_read_bit(bit_set_t* bit_set){
    byte_slice_t* slice = &(bit_set->byte_slice);

    bool bit = byte_slice_read_bit(slice);

    if(slice->len == 0){//slice is empty -> copy next byte to slice
        slice->bits = bit_set->bytes[bit_set->read_id];
        slice->len = BYTE_SLICE_BIT_COUNT;
        bit_set->read_id++;
    }

    return bit;
}

void bit_set_write_slice(bit_set_t* bit_set, byte_slice_t slice){
    //write slice bit by bit
    while(slice.len > 0){
        bit_set_write_bit(bit_set, byte_slice_read_bit(&slice));
    }
}

void bit_set_write_byte(bit_set_t* bit_set, unsigned char byte){
    byte_slice_t slice;
    slice.bits = byte;
    slice.len = BYTE_SLICE_BIT_COUNT;
    bit_set_write_slice(bit_set, slice);
}

unsigned char bit_set_read_byte(bit_set_t* bit_set){
    //perform 8 bit reads
   byte_slice_t slice;
   byte_slice_init(&slice);
   while(slice.len < BYTE_SLICE_BIT_COUNT){
        byte_slice_write_bit(&slice, bit_set_read_bit(bit_set));
   }
   return slice.bits; 
}

std::vector<unsigned char> bit_set_extract_bytes(bit_set_t* bit_set){
    bit_set->owning_bytes = false;
    return bit_set->bytes;
}

void bit_set_write_code(bit_set_t* bit_set, bit_code_t code){
    for(size_t i = 0; i < code.bits.size(); i++){
        bit_set_write_bit(bit_set, code.bits[i]);
    }
}