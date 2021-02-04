cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

include(example_auto_set_url.cmake)

add_executable(enclosure enclosure.cc)

# Logger
add_library(logger logger.cc logger.h)
target_link_libraries(logger pico_stdlib)

# uComs
add_library(uComsDecode STATIC ./uComs/generated_files/generated_ucoms_decode.cc ./uComs/generated_files/generated_ucoms.cc)
add_library(uComsUtils STATIC ./uComs/generated_files/generated_ucoms.cc)
target_include_directories(enclosure PUBLIC ./uComs/generated_files/)

# Link all libraries into enclosure executable
target_link_libraries(enclosure pico_stdlib logger uComsDecode uComsUtils)

# enable usb output, disable uart output
pico_enable_stdio_usb(enclosure 1)
pico_enable_stdio_uart(enclosure 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(enclosure)

# add url via pico_set_program_url
example_auto_set_url(enclosure)
