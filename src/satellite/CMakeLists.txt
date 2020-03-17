cmake_minimum_required(VERSION 3.10.2)

project(satellite_src)

# Adding exception
set(
  SATELLITE_PARSE_EXCEPTION_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/satellite_parse_exception
)
if(NOT TARGET satellite_parse_exception)
  add_subdirectory(${SATELLITE_PARSE_EXCEPTION_INCLUDE_DIR})
endif()

set(SOURCE satellite.cpp satellite_data.cpp)

add_library(satellite ${SOURCE})

set_target_properties(satellite PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON
)

target_include_directories(satellite PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${SATELLITE_PARSE_EXCEPTION_INCLUDE_DIR}
)

target_link_libraries(satellite
  satellite_parse_exception
)
