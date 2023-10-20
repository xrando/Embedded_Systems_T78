cmake_minimum_required(VERSION 3.20)
project(Embedded_Systems_78)

include(pico_sdk_import.cmake)
include(example_auto_set_url.cmake)

project(embedded_team78 C CXX ASM)

project(embedded_team78 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)

set(PROJECT_PATH ${PROJECT_SOURCE_DIR})
set(PROGRAM_NAME embedded_team78)

pico_sdk_init()

add_subdirectory(barcode_sensor)
add_subdirectory(line_sensor)
add_subdirectory(motor)
add_subdirectory(web-ui)