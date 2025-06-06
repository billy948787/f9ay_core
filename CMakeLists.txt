﻿cmake_minimum_required(VERSION 3.10)

project(f9ay)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# download gtest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY
        https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# add the include directory
include_directories(include)


file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")
# add the source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_compile_definitions(
        DEBUG=1
)
add_executable(f9ay ${SOURCES} ${HEADERS})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(f9ay PRIVATE /FAcs /Qpar /utf-8 /Qvec-report:1)
    target_link_libraries(f9ay PRIVATE user32 gdi32)
elseif (CMAKE_CXXCOMPILER_ID STREQUAL "CLANG")
    target_compile_options(f9ay PRIVATE -fverbose-asm --save-temps -fexperimental-library)
else ()
    # target_compile_definitions(f9ay PRIVATE -fallow-extended-attribute-placement)
endif ()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(f9ay PRIVATE -mavx2)
    elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(f9ay PRIVATE /arch:AVX2)
    endif ()
endif ()


enable_testing()
add_executable(lz77_test test/lz77_test.cpp)
target_include_directories(lz77_test PRIVATE ${GTEST_INCLUDE_DIRS} include/lz77_compress.hpp)
target_link_libraries(lz77_test PRIVATE gtest_main)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(lz77_test PRIVATE /FAcs $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>)
    target_link_libraries(lz77_test PRIVATE user32 gdi32)
endif ()

add_test(NAME lz77_test COMMAND lz77_test)


add_executable(huffman_test test/huffman_test.cpp)
target_include_directories(huffman_test PRIVATE ${GTEST_INCLUDE_DIRS} include/huffman_coding.hpp)
target_link_libraries(huffman_test PRIVATE gtest_main)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(huffman_test PRIVATE /FAcs $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>)
    target_link_libraries(huffman_test PRIVATE user32 gdi32)
endif ()
add_test(NAME huffman_test COMMAND huffman_test)