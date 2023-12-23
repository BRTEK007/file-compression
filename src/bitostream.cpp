#include "bitostream.hpp"

BitOstream::BitOstream(std::ostream& s) : stream(s), byteSlice(){};

void BitOstream::writeBit(bool bit){
    //TODO better ByteSlice
    byteSlice.write_bit(bit);
    if(byteSlice.len == BYTE_SLICE_BIT_COUNT){
        stream << byteSlice.bits;
        byteSlice = ByteSlice();
    }
}

void BitOstream::flush(){
    unsigned char byte = byteSlice.bits;
    byte = byte << (BYTE_SLICE_BIT_COUNT-byteSlice.len); 
    stream << byte;
}

void BitOstream::writeByte(unsigned char byte){
    ByteSlice slice;
    slice.set_byte(byte);
    writeSlice(slice);
}

void BitOstream::writeSlice(ByteSlice slice){
    //write slice bit by bit
    while(slice.len > 0){
        writeBit(slice.read_bit());
    }
}

void BitOstream::writeBitcode(BitCode bc){
    while(bc.size() > 0){
        writeBit(bc.readBit());
    }
}