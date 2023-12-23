cmake_minimum_required(VERSION 3.7)

project(file_compression)

include_directories(include)

add_executable(byte-compressor)

target_compile_options(byte-compressor PRIVATE -Werror)

target_sources(byte-compressor PRIVATE 
    src/bit_code.cpp 
    src/main.cpp 
    src/node_queue.cpp 
    src/tree.cpp 
    src/byte_slice.cpp
    src/io.cpp
    src/byte_freq.cpp
    src/compress.cpp
    src/decompress.cpp
    src/bitostream.cpp
    src/bitistream.cpp)

#add_executable(unit_tests)

# target_compile_options(unit_tests PRIVATE -Werror -Wall -Wextra)

#target_sources(unit_tests PRIVATE 
    #src/unit_tests.cpp
    #src/byte_slice.cpp
    #src/bit_set.cpp
    #src/bit_code.cpp
    #src/node_queue.cpp
    #src/tree.cpp
#)