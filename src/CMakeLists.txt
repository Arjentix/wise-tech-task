cmake_minimum_required(VERSION 3.16.5)

project(wise_tech_task_src)

# Adding satellite
set(
  SATELLITE_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/satellite
)
if(NOT TARGET satellite)
  add_subdirectory(${SATELLITE_INCLUDE_DIR})
endif()

# Adding les
set(
  LES_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/les
)
if(NOT TARGET les)
  add_subdirectory(${LES_INCLUDE_DIR})
endif()

set(SOURCE_EXE main.cpp)

add_executable(wise_tech_task ${SOURCE_EXE})

set_target_properties(wise_tech_task PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON
)

target_link_libraries(wise_tech_task
  satellite
  les
)

# Installing
install(TARGETS wise_tech_task DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
