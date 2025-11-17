/**
 * @file gxepd2_zephyr.h
 * @brief Zephyr-specific helpers for GxEPD2 library
 *
 * This file provides helper functions for initializing GxEPD2 displays
 * from device tree configuration on Zephyr RTOS.
 */

#ifndef _GXEPD2_ZEPHYR_H_
#define _GXEPD2_ZEPHYR_H_

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include "../hal/zephyr_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Pin ID definitions for GPIO HAL */
#define GXEPD2_PIN_CS    0
#define GXEPD2_PIN_DC    1
#define GXEPD2_PIN_RST   2
#define GXEPD2_PIN_BUSY  3

/**
 * @brief Initialize GxEPD2 HAL from device tree
 *
 * This function initializes the SPI and GPIO pins for the e-paper display
 * based on the device tree configuration.
 *
 * @param epaper_node Device tree node for the epaper device
 * @return true if initialization successful, false otherwise
 */
bool gxepd2_init_from_dt(const struct device *epaper_node);

/**
 * @brief Get display properties from device tree
 *
 * @param epaper_node Device tree node for the epaper device
 * @param width Pointer to store display width
 * @param height Pointer to store display height
 * @param panel_type Pointer to store panel type string
 * @return true if successful, false otherwise
 */
bool gxepd2_get_dt_properties(const struct device *epaper_node,
                               uint16_t *width, uint16_t *height,
                               const char **panel_type);

#ifdef __cplusplus
}
#endif

/* C++ helper macros for device tree access */
#ifdef __cplusplus

#define DT_EPAPER DT_NODELABEL(epaper)

/* Check if epaper node exists in device tree */
#if DT_NODE_EXISTS(DT_EPAPER)
  #define GXEPD2_DT_AVAILABLE 1

  /* Get device tree properties */
  #define GXEPD2_DT_WIDTH      DT_PROP(DT_EPAPER, width)
  #define GXEPD2_DT_HEIGHT     DT_PROP(DT_EPAPER, height)
  #define GXEPD2_DT_PANEL_TYPE DT_PROP(DT_EPAPER, panel_type)

  /* Get SPI device */
  #define GXEPD2_DT_SPI_DEV    DEVICE_DT_GET(DT_BUS(DT_EPAPER))

#else
  #define GXEPD2_DT_AVAILABLE 0
#endif

#endif /* __cplusplus */

#endif /* _GXEPD2_ZEPHYR_H_ */
