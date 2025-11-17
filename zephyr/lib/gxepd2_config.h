/**
 * @file gxepd2_config.h
 * @brief Auto-configuration system for GxEPD2 based on Kconfig selections
 *
 * This file automatically configures the display and pins based on Kconfig choices,
 * making integration as simple as selecting your board and display model.
 */

#ifndef _GXEPD2_CONFIG_H_
#define _GXEPD2_CONFIG_H_

#include <zephyr/kernel.h>
#include "hal/zephyr_hal.h"

/* Forward declarations */
class GxEPD2_EPD;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Simple initialization function - automatically configures everything
 *
 * Call this once at startup. It will:
 * 1. Initialize SPI and GPIO from device tree
 * 2. Create the appropriate display driver instance
 * 3. Initialize the display hardware
 *
 * @return 0 on success, negative error code on failure
 */
int gxepd2_init(void);

/**
 * @brief Get the configured display instance
 *
 * @return Pointer to the display driver instance, or NULL if not initialized
 */
GxEPD2_EPD* gxepd2_get_display(void);

/**
 * @brief Get display dimensions
 *
 * @param width Pointer to store width (can be NULL)
 * @param height Pointer to store height (can be NULL)
 */
void gxepd2_get_dimensions(uint16_t *width, uint16_t *height);

/**
 * @brief Print configuration info to console
 */
void gxepd2_print_config(void);

#ifdef __cplusplus
}
#endif

/* Display model auto-configuration */
#if defined(CONFIG_GXEPD2_DISPLAY_154)
  #define GXEPD2_DISPLAY_WIDTH  200
  #define GXEPD2_DISPLAY_HEIGHT 200
  #define GXEPD2_PANEL_NAME     "1.54\" GDEP015OC1"
  #define GXEPD2_DRIVER_154

#elif defined(CONFIG_GXEPD2_DISPLAY_213)
  #define GXEPD2_DISPLAY_WIDTH  250
  #define GXEPD2_DISPLAY_HEIGHT 122
  #define GXEPD2_PANEL_NAME     "2.13\" GDEW0213T5D"
  #define GXEPD2_DRIVER_213

#elif defined(CONFIG_GXEPD2_DISPLAY_290)
  #define GXEPD2_DISPLAY_WIDTH  296
  #define GXEPD2_DISPLAY_HEIGHT 128
  #define GXEPD2_PANEL_NAME     "2.9\" GDEW029T5"
  #define GXEPD2_DRIVER_290

#elif defined(CONFIG_GXEPD2_DISPLAY_420)
  #define GXEPD2_DISPLAY_WIDTH  400
  #define GXEPD2_DISPLAY_HEIGHT 300
  #define GXEPD2_PANEL_NAME     "4.2\" GDEW042T2"
  #define GXEPD2_DRIVER_420

#elif defined(CONFIG_GXEPD2_DISPLAY_CUSTOM)
  #define GXEPD2_PANEL_NAME     "Custom (Device Tree)"
  #define GXEPD2_DRIVER_CUSTOM
#endif

/* Board auto-detection and pin configuration */
#if defined(CONFIG_GXEPD2_BOARD_AUTO)
  /* Auto-detect from build configuration */
  #if defined(CONFIG_BOARD_NRF52840DK_NRF52840)
    #define GXEPD2_BOARD_NRF52840DK
  #elif defined(CONFIG_BOARD_NRF5340DK_NRF5340_CPUAPP)
    #define GXEPD2_BOARD_NRF5340DK
  #elif defined(CONFIG_BOARD_NRF54L15DK_NRF54L15_CPUAPP)
    #define GXEPD2_BOARD_NRF54L15DK
  #else
    #warning "Board auto-detection failed - using custom configuration"
    #define GXEPD2_BOARD_CUSTOM
  #endif

#elif defined(CONFIG_GXEPD2_BOARD_NRF52840DK)
  #define GXEPD2_BOARD_NRF52840DK

#elif defined(CONFIG_GXEPD2_BOARD_NRF5340DK)
  #define GXEPD2_BOARD_NRF5340DK

#elif defined(CONFIG_GXEPD2_BOARD_NRF54L15DK)
  #define GXEPD2_BOARD_NRF54L15DK

#elif defined(CONFIG_GXEPD2_BOARD_CUSTOM)
  #define GXEPD2_BOARD_CUSTOM
#endif

/* Board-specific configurations with overlay paths */
#if defined(GXEPD2_BOARD_NRF52840DK) || defined(GXEPD2_BOARD_NRF5340DK)
  #define GXEPD2_BOARD_NAME "nRF52840/5340 DK"
  #define GXEPD2_USES_SPI1
  #define GXEPD2_PIN_INFO "SPI1: SCK=P1.01 MOSI=P1.02, CS=P1.03, DC=P1.04, RST=P1.05, BUSY=P1.06"

#elif defined(GXEPD2_BOARD_NRF54L15DK)
  #define GXEPD2_BOARD_NAME "nRF54L15 DK"
  #define GXEPD2_USES_SPI20
  #define GXEPD2_PIN_INFO "SPI20: SCK=P1.04 MOSI=P1.05, CS=P1.06, DC=P1.07, RST=P1.08, BUSY=P1.09"

#elif defined(GXEPD2_BOARD_CUSTOM)
  #define GXEPD2_BOARD_NAME "Custom"
  #define GXEPD2_PIN_INFO "See device tree overlay"
#endif

#endif /* _GXEPD2_CONFIG_H_ */
