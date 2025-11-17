# CMakeLists.txt for GxEPD2 Basic Sample Application

cmake_minimum_required(VERSION 3.20.0)

# Find Zephyr package
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Project name
project(gxepd2_basic)

# Application source files
target_sources(app PRIVATE src/main.cpp)

# Include GxEPD2 library
# The library is included via the module.yml mechanism
