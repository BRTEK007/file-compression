cmake_minimum_required(VERSION 3.7)

project(file_compression)

include_directories(include)

add_executable(compressor)

file(GLOB compressor_SRC "src/*.c")

target_sources(compressor PRIVATE ${compressor_SRC})

# target_link_libraries(trafsim sfml-system)