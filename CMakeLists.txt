cmake_minimum_required(VERSION 3.7)

project(file_compression)

include_directories(include)

add_executable(compressor)

# file(GLOB compressor_SRC "src/*.c")

# target_sources(compressor PRIVATE ${compressor_SRC})

target_sources(compressor PRIVATE 
    src/bit_set.c 
    src/main.c 
    src/node_queue.c 
    src/tree.c 
    src/byte_slice.c)

add_executable(tests)

target_sources(tests PRIVATE 
    src/tests.c
    src/byte_slice.c
    src/bit_set.c
)
# target_link_libraries(trafsim sfml-system)