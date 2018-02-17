cmake_minimum_required(VERSION 2.8)

project(huffman)

include_directories(${HUFFMAN_SOURCE_DIR})

add_library(huffman_lib
        decoder.cpp
        encoder.cpp
        frequency_detector.cpp
        huffman_cod.h
    )

install(TARGETS huffman_lib DESTINATION lib)
install(FILES
        huffman_con.h
        DESTINATION include/huffman_lib)

add_executable(huffman
    main.cpp
 )

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -std=c++11 -pedantic")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined")
endif()
target_link_libraries(huffman huffman_lib)
