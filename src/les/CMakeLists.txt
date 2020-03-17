cmake_minimum_required(VERSION 3.10.2)

project(les_src)

set(SOURCE les.cpp)

add_library(les ${SOURCE})

set_target_properties(les PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON
)

target_include_directories(les PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
