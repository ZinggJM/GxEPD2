# CMakeLists.txt for GxEPD2 Zephyr Module
#
# This module provides e-paper display support for Zephyr RTOS 4.2.1
# using the GxEPD2 library ported from Arduino.

if(CONFIG_GXEPD2)

zephyr_library()

# Include directories
zephyr_include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# HAL source files
zephyr_library_sources(
    hal/zephyr_hal.cpp
)

# Core library source files
zephyr_library_sources(
    lib/GxEPD2_EPD.cpp
    lib/gxepd2_zephyr.cpp
    lib/gxepd2_config.cpp
)

# Display drivers (conditionally compiled based on Kconfig)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_DISPLAY_154 lib/epd/GxEPD2_154.cpp)

# Compiler options for C++
zephyr_library_compile_options(
    -std=c++11
    -fno-exceptions
    -fno-rtti
)

endif() # CONFIG_GXEPD2
