cmake_minimum_required(VERSION 3.10.2)

project(wise_tech_task)

add_subdirectory(src)

# Installing
install(TARGETS wise_tech_task DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)