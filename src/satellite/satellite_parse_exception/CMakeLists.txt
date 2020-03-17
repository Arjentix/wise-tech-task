cmake_minimum_required(VERSION 3.10.2)

project(exception_src)

set(SOURCE satellite_parse_exception.cpp)

add_library(satellite_parse_exception ${SOURCE})

target_include_directories(satellite_parse_exception PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
