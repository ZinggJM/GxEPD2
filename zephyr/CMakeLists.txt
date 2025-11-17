# CMakeLists.txt - Auto-includes selected display driver from src/epd/

if(CONFIG_GXEPD2)

zephyr_library()

# Include directories - use ORIGINAL source location!
zephyr_include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src      # Original Arduino library src/
    ${CMAKE_CURRENT_SOURCE_DIR}/hal         # Zephyr HAL
    ${CMAKE_CURRENT_SOURCE_DIR}/lib         # Zephyr helpers
)

# HAL - Makes original Arduino code work on Zephyr
zephyr_library_sources(
    hal/zephyr_hal.cpp
)

# Core library
zephyr_library_sources(
    lib/gxepd2_zephyr.cpp
    lib/gxepd2_config.cpp
    ../src/GxEPD2_EPD.cpp                   # Original base class!
)

# Auto-include selected display driver from ORIGINAL src/epd/
# No copying needed - use directly!

zephyr_library_sources_ifdef(CONFIG_GXEPD2_102          ../src/epd/GxEPD2_102.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_150_BN       ../src/epd/GxEPD2_150_BN.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_154          ../src/epd/GxEPD2_154.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_154_D67      ../src/epd/GxEPD2_154_D67.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_154_M09      ../src/epd/GxEPD2_154_M09.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_154_M10      ../src/epd/GxEPD2_154_M10.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_154_T8       ../src/epd/GxEPD2_154_T8.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213          ../src/epd/GxEPD2_213.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213_B72      ../src/epd/GxEPD2_213_B72.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213_B73      ../src/epd/GxEPD2_213_B73.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213_B74      ../src/epd/GxEPD2_213_B74.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213_BN       ../src/epd/GxEPD2_213_BN.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213_M21      ../src/epd/GxEPD2_213_M21.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213_T5D      ../src/epd/GxEPD2_213_T5D.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_213_flex     ../src/epd/GxEPD2_213_flex.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_260          ../src/epd/GxEPD2_260.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_260_M01      ../src/epd/GxEPD2_260_M01.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_266_BN       ../src/epd/GxEPD2_266_BN.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_270          ../src/epd/GxEPD2_270.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_290          ../src/epd/GxEPD2_290.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_290_T5       ../src/epd/GxEPD2_290_T5.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_290_T5D      ../src/epd/GxEPD2_290_T5D.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_290_T94      ../src/epd/GxEPD2_290_T94.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_290_T94_V2   ../src/epd/GxEPD2_290_T94_V2.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_290_M06      ../src/epd/GxEPD2_290_M06.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_290_BS       ../src/epd/GxEPD2_290_BS.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_371          ../src/epd/GxEPD2_371.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_420          ../src/epd/GxEPD2_420.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_420_M01      ../src/epd/GxEPD2_420_M01.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_420_M02      ../src/epd/GxEPD2_420_M02.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_437          ../src/epd/GxEPD2_437.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_565          ../src/epd/GxEPD2_565.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_583          ../src/epd/GxEPD2_583.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_583_T8       ../src/epd/GxEPD2_583_T8.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_750          ../src/epd/GxEPD2_750.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_750_T7       ../src/epd/GxEPD2_750_T7.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_1160_T91     ../src/epd/GxEPD2_1160_T91.cpp)
zephyr_library_sources_ifdef(CONFIG_GXEPD2_1248         ../src/epd/GxEPD2_1248.cpp)

# C++ options
zephyr_library_compile_options(
    -std=c++11
    -fno-exceptions
    -fno-rtti
)

endif() # CONFIG_GXEPD2
