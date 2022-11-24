cmake_minimum_required(VERSION 3.13) # CMAKE (heading)
include(pico_sdk_import.cmake) # CMAKE -> PICO SDK (include)

project(Pioscope C CXX ASM) # CMAKE ("project")
pico_sdk_init() # PICO SDK (initialization)

add_executable(pioscope) # CMAKE (target)

target_sources(pioscope PRIVATE pioscope.c) # CMAKE -> C (compiler)
target_link_libraries(pioscope PRIVATE pico_stdlib hardware_pio hardware_dma) # CMAKE -> C (linker)

pico_enable_stdio_usb(pioscope 1) # PICO SDK (USB)
pico_enable_stdio_uart(pioscope 0) # PICO SDK (USB)

pico_add_extra_outputs(pioscope) # PICO SDK (UF2)
