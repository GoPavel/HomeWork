cmake_minimum_required(VERSION 2.8)

project(huffman)

include_directories(${HUFFMAN_SOURCE_DIR})

add_executable(huffman
    main.cpp
    frequency_detector.cpp
    huffman_lib.h
    encoder.cpp
    decoder.cpp
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
