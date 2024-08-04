#include "byte_freq.hpp"

#include <algorithm>
namespace oddcod
{
  // returns array of byte_frequencies from input byte buffer
  std::vector<ByteFreq> findByteFrequencies(Input &input)
  {
    std::array<ByteFreq, 256> arr;

    for (int i = 0; i < 256; i++)
    {
      arr[i].byte = (unsigned char)i;
      arr[i].freq = 0;
    }

    // count bytes
    while (!input.eof())
    {
      auto byte = input.readAligned();
      arr[byte].freq++;
    }

    // sort by frequencies
    std::sort(arr.begin(), arr.end(),
              [](const ByteFreq &a, const ByteFreq &b)
              { return a.freq > b.freq; });

    std::vector<ByteFreq> bf_arr;

    // add bytes with atleast 1 occurance to new array
    for (ByteFreq bf : arr)
    {
      if (bf.freq == 0)
        break;
      bf_arr.push_back(bf);
    }

    return bf_arr;
  }
};