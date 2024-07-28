cmake_minimum_required(VERSION 3.7)

project(odd_coding_lib)

include_directories(include)
include_directories(.)

add_library(oddcod)

target_compile_options(oddcod PRIVATE -Werror -Wall -Wextra -O2)

file(GLOB ODDCOD_SRC src/*.cpp)

target_sources(oddcod PRIVATE ${ODDCOD_SRC})



add_executable(unit_tests)

target_compile_options(unit_tests PRIVATE -Werror -Wall -Wextra)

target_link_libraries(unit_tests gtest)

target_sources(unit_tests PRIVATE ${ODDCOD_SRC} unit_tests.cpp)