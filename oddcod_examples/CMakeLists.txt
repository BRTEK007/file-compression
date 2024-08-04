cmake_minimum_required(VERSION 3.7)

project(oddcod_exaples)

add_executable(file-compression)

target_compile_options(file-compression PRIVATE -Werror -Wall -Wextra -O2)

target_sources(file-compression PRIVATE file-compression.cpp)


include_directories(../oddcod)

target_link_libraries(file-compression PRIVATE ${CMAKE_SOURCE_DIR}/../oddcod/build/liboddcod.a)
