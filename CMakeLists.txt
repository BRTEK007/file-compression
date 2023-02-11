cmake_minimum_required(VERSION 3.7)

project(file_compression)

include_directories(include)

add_executable(byte-compressor)

target_sources(byte-compressor PRIVATE 
    src/bit_set.c 
    src/main.c 
    src/node_queue.c 
    src/tree.c 
    src/byte_slice.c
    src/io.c
    src/byte_freq.c
    src/compress.c
    src/decompress.c)

add_executable(tests)

target_sources(tests PRIVATE 
    src/tests.c
    src/byte_slice.c
    src/bit_set.c
)
# target_link_libraries(trafsim sfml-system)