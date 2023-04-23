#include "byte_freq.hpp"
#include <stdlib.h>
#include <array>
#include <algorithm>

//returns array of byte_frequencies from input byte buffer
std::vector<byte_freq_t> get_byte_frequencies(const std::vector<unsigned char>& bytes_in){ 
  std::array<byte_freq_t, 256> arr;
  
  for(int i = 0; i < 256; i++){//init array
    arr[i].byte = (unsigned char)i;
    arr[i].freq = 0;
  }

  //count bytes
  for(auto byte : bytes_in){
    arr[byte].freq++;
  }

  //sort by frequencies
  std::sort(arr.begin(), arr.end(),
  [](const byte_freq_t& a, const byte_freq_t& b) {return a.freq > b.freq;});

  std::vector<byte_freq_t> bf_arr;
  
  //add bytes with atleast 1 occurance to new array
  for(byte_freq_t bf : arr){
    if(bf.freq == 0) break;
    bf_arr.push_back(bf);
  }
  
  return bf_arr;
}