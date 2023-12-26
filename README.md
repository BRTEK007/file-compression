# file-compression

c++ file compression and decompression, using Huffman coding

note: project is in a state of being converted from plain C

## example

```console
> ./byte-compressor -c ../tests/files/22_total_5_unique.txt out.bin
-------------------------
COMPRESSING 21 BYTES, 5 UNIQUE
-------------------------
BYTE   | FREQUENCY | CODE
-------------------------
70 (F) |       4.8 | 1100
77 (M) |       4.8 | 1101
71 (G) |      19.0 | 111
88 (X) |      23.8 | 10
65 (A) |      47.6 | 0
-------------------------
size after compression: 17, 19.0% reduction
-------------------------
```

```console
> python3 tester.py
PASSED files/22_total_5_unique.txt
PASSED files/iliad.txt
PASSED files/1000_digits.bin
PASSED files/1000_lowercase.bin
PASSED files/1000_printable.bin
FAILED files/10000_all.bin (DECOMPRESSION)
FAILED files/lena.tiff (DECOMPRESSION)
FAILED files/mozart_symphony_40.wav (DECOMPRESSION)
```

## testing

tests directory contains python script that tests compression then decompression and compares the files

unit tests in src/unit_tests.cpp, unit_tests Cmake target

## build

```console
> mkdir build
> cd build
> cmake ..
> make
```

## TODO:

use exceptions
googletest
better python script, timeout, .log, automatic file search
args handling
comments fix, TODOS...
optimize?
readme
testfiles

# test images: https://imagecompression.info/test_images/
