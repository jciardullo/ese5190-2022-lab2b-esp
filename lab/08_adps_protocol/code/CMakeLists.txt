cmake_minimum_required(VERSION 3.13) # CMAKE (heading)
include(pico_sdk_import.cmake) # CMAKE -> PICO SDK (include)

project(apds C CXX ASM) # CMAKE ("project")
pico_sdk_init() # PICO SDK (initialization)

add_executable(apds) # CMAKE (target)

pico_generate_pio_header(apds ${CMAKE_CURRENT_LIST_DIR}/i2c.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated) # PICO SDK (PIO)

target_sources(apds PRIVATE apds.c pio_i2c.c) # CMAKE -> C (compiler)
target_link_libraries(apds PRIVATE pico_stdlib hardware_pio) # CMAKE -> C (linker)

pico_enable_stdio_usb(apds 1) # PICO SDK (USB)
pico_enable_stdio_uart(apds 0) # PICO SDK (USB)

pico_add_extra_outputs(apds) # PICO SDK (UF2)
