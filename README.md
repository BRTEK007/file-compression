# File-compression

c++ file compression and decompression, using Huffman coding

# Project structure

- **oddcod** coding library with unit tests written in google test
- **oddcod_examples** minimal file compression program, with python tests

# Minimal example usage of library

```
  std::ifstream inStream;
  std::ofstream outStream;

  inStream.open(input_filename, std::ifstream::binary);
  outStream.open(output_filename, std::ofstream::binary);

  if (!outStream.is_open())
  {
    fprintf(stderr, "ERROR: could not open file: %s\n", output_filename);
    exit(2);
  }
  if (!inStream.is_open())
  {
    fprintf(stderr, "ERROR: could not open file: %s\n", input_filename);
    exit(2);
  }

  oddcod::Result result;

  if(doCompress){
    result = oddcod::huffman::encode(inStream, outStream);
  }else{
    result = oddcod::huffman::decode(inStream, outStream);
  }

  inStream.close();
  outStream.close();
```

# Build
- uses cmake and make
- `build_all.sh` script provided

# Testing
- `run_unit_tests.sh` and `run_e2e_tests.sh` scripts provided
unit
- unit tests for oddcod library, use google test
- python tests run file compression and decompression program, and comprare the results with original

## Example unit tests run:
```
bash run_unit_tests.sh
```
```
[==========] Running 6 tests from 5 test suites.
[----------] Global test environment set-up.
[----------] 1 test from ByteSlice
[ RUN      ] ByteSlice.WriteReadEqual
[       OK ] ByteSlice.WriteReadEqual (0 ms)
[----------] 1 test from ByteSlice (0 ms total)

[----------] 1 test from BitCode
[ RUN      ] BitCode.WriteReadEqual
[       OK ] BitCode.WriteReadEqual (0 ms)
[----------] 1 test from BitCode (0 ms total)

[----------] 1 test from Tree
[ RUN      ] Tree.CreateFromAllBytesValid
[       OK ] Tree.CreateFromAllBytesValid (0 ms)
[----------] 1 test from Tree (0 ms total)

[----------] 1 test from BitWriter
[ RUN      ] BitWriter.WriteCorrectBitsBytes
[       OK ] BitWriter.WriteCorrectBitsBytes (0 ms)
[----------] 1 test from BitWriter (0 ms total)

[----------] 2 tests from BitReader
[ RUN      ] BitReader.ReadCorrectBitsBytes
[       OK ] BitReader.ReadCorrectBitsBytes (0 ms)
[ RUN      ] BitReader.ReadAligned
[       OK ] BitReader.ReadAligned (0 ms)
[----------] 2 tests from BitReader (0 ms total)

[----------] Global test environment tear-down
[==========] 6 tests from 5 test suites ran. (0 ms total)
[  PASSED  ] 6 tests.
```

## Example file compression tests run

```
bash run_e2e_tests.sh
```

```
both/1000B_digits                      1000B    PASSED 4.94ms (deflated 56.3%)
both/1000B_printable                   1000B    PASSED 3.86ms (deflated 6.2%)
both/10000B_all                       10000B    PASSED 7.34ms (deflated -3.2%)
both/22B_5unique                         21B    PASSED 4.81ms (deflated 33.33%)
both/iliad                          1161151B    PASSED 130.41ms (deflated 40.5%)
both/1000B_lowercase                   1000B    PASSED 7.21ms (deflated 37.5%)
both/1B                                   1B    PASSED 5.47ms (deflated -200.0%)
both/img/fireworks.pgm              7375889B    PASSED 667.1ms (deflated 52.88%)
both/img/readme.txt                    2038B    PASSED 4.35ms (deflated 38.32%)
```

# test images: https://imagecompression.info/test_images/
