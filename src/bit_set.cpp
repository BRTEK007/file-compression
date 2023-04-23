#include "bit_set.hpp"

#include <vector>
#include <stdio.h>

BitSet::BitSet(){
    byte_slice_init(&(byte_slice));
    read_id = 0;
    owning_bytes = true;
}

BitSet::~BitSet(){}

void BitSet::create_from_bytes(std::vector<unsigned char> bytes){
    byte_slice_init(&(byte_slice));
    this->bytes = bytes;
    read_id = 0;
    owning_bytes = false;
}

void BitSet::end_write(){
    //push byte_slice to bytes, bit by bit
    unsigned char byte = byte_slice.bits;
    byte = byte << (BYTE_SLICE_BIT_COUNT-byte_slice.len);
    
    bytes.push_back(byte);
}

void BitSet::begin_read(){
    byte_slice_t* slice = &(byte_slice);
    read_id = 0;
    slice->bits = bytes[read_id];
    slice->len = BYTE_SLICE_BIT_COUNT;
    read_id++;
}

void BitSet::write_bit(bool bit){
    //write bit to slice, if slice full write slice to bytes
    byte_slice_t* slice = &(byte_slice);
    byte_slice_write_bit(slice, bit);
    if(slice->len == BYTE_SLICE_BIT_COUNT){//is full, can be added to bytes
        bytes.push_back(slice->bits);
        byte_slice_init(slice);
    }
}

void BitSet::write_byte(unsigned char byte){
    byte_slice_t slice;
    slice.bits = byte;
    slice.len = BYTE_SLICE_BIT_COUNT;
    this->write_slice(slice);
}

void BitSet::write_slice(byte_slice_t slice){
    //write slice bit by bit
    while(slice.len > 0){
        this->write_bit(byte_slice_read_bit(&slice));
    }
}

void BitSet::write_bitcode(BitCode bc){
    while(bc.size() > 0){
        this->write_bit(bc.readBit());
    }
}

bool BitSet::read_bit(){
    byte_slice_t* slice = &(byte_slice);

    bool bit = byte_slice_read_bit(slice);

    if(slice->len == 0){//slice is empty -> copy next byte to slice
        slice->bits = bytes[read_id];
        slice->len = BYTE_SLICE_BIT_COUNT;
        read_id++;
    }

    return bit;
}

unsigned char BitSet::read_byte(){
    //perform 8 bit reads
   byte_slice_t slice;
   byte_slice_init(&slice);
   while(slice.len < BYTE_SLICE_BIT_COUNT){
        byte_slice_write_bit(&slice, this->read_bit());
   }
   return slice.bits; 
}

std::vector<unsigned char> BitSet::extract_bytes(){
    owning_bytes = false;
    return bytes;
}