#include "bit_set.hpp"
#include "byte_slice.hpp"
#include <vector>
#include <stdio.h>

BitSet::BitSet(){
    read_id = 0;
    owning_bytes = true;
}

BitSet::~BitSet(){}

void BitSet::create_from_bytes(std::vector<unsigned char> bytes){
    byte_slice = ByteSlice();
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
    read_id = 0;
    byte_slice.set_byte(bytes[read_id]);
    read_id++;
}

void BitSet::write_bit(bool bit){
    //write bit to slice, if slice full write slice to bytes
    byte_slice.write_bit(bit);
    if(byte_slice.len == BYTE_SLICE_BIT_COUNT){//is full, can be added to bytes
        bytes.push_back(byte_slice.bits);
        byte_slice = ByteSlice();
    }
}

void BitSet::write_byte(unsigned char byte){
    ByteSlice slice;
    slice.set_byte(byte);
    this->write_slice(slice);
}

void BitSet::write_slice(ByteSlice slice){
    //write slice bit by bit
    while(slice.len > 0){
        this->write_bit(slice.read_bit());
    }
}

void BitSet::write_bitcode(BitCode bc){
    while(bc.size() > 0){
        this->write_bit(bc.readBit());
    }
}

bool BitSet::read_bit(){
    bool bit = byte_slice.read_bit();

    if(byte_slice.len == 0){//slice is empty -> copy next byte to slice
        byte_slice.set_byte(bytes[read_id]);
        read_id++;
    }

    return bit;
}

unsigned char BitSet::read_byte(){
    //perform 8 bit reads
   ByteSlice slice;
   while(slice.len < BYTE_SLICE_BIT_COUNT){
        slice.write_bit(this->read_bit());
   }
   return slice.bits; 
}

std::vector<unsigned char> BitSet::extract_bytes(){
    owning_bytes = false;
    return bytes;
}