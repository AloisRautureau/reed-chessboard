cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(rcc
    VERSION 0.1.0
    DESCRIPTION "Code to make a smart chessboard out of a reed switch matrix and an RP2040 based controller"
    LANGUAGES C CXX ASM
    )
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()

add_executable(rcc
    main.c
    read.h
    bitboard.h
    move.h
    logic.h
    communication.h
    )

target_link_libraries(rcc pico_stdlib)

pico_add_extra_outputs(rcc)

pico_enable_stdio_usb(rcc 1)
pico_enable_stdio_uart(rcc 0)