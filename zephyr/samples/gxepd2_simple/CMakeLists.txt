# CMakeLists.txt for GxEPD2 Simple Sample
# This sample demonstrates the simplest possible use of GxEPD2

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gxepd2_simple)

target_sources(app PRIVATE src/main.cpp)
