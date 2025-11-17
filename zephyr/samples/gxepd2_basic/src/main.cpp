/**
 * @file main.cpp
 * @brief GxEPD2 Basic Example for Zephyr RTOS
 *
 * This example demonstrates basic usage of the GxEPD2 library on Zephyr.
 * It shows how to initialize the display and perform basic drawing operations.
 *
 * Hardware Requirements:
 * - Nordic nRF52840DK, nRF5340DK, or nRF54L15DK
 * - Good Display or Waveshare e-paper display
 * - Proper wiring as defined in the device tree overlay
 *
 * Build for nRF52840DK:
 *   west build -b nrf52840dk_nrf52840
 *
 * Build for nRF5340DK:
 *   west build -b nrf5340dk_nrf5340_cpuapp
 *
 * Build for nRF54L15DK:
 *   west build -b nrf54l15dk_nrf54l15_cpuapp
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "gxepd2_zephyr.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* Simple framebuffer example - 1.54" display is 200x200 pixels */
/* Each pixel is 1 bit (black or white), so we need width * height / 8 bytes */
#define DISPLAY_WIDTH  200
#define DISPLAY_HEIGHT 200
#define FRAMEBUFFER_SIZE ((DISPLAY_WIDTH * DISPLAY_HEIGHT) / 8)

static uint8_t framebuffer[FRAMEBUFFER_SIZE];

/**
 * @brief Draw a simple test pattern to framebuffer
 */
void draw_test_pattern(void)
{
    /* Clear framebuffer (white) */
    memset(framebuffer, 0xFF, FRAMEBUFFER_SIZE);

    /* Draw a black border */
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        /* Top border */
        int byte_idx_top = x / 8;
        int bit_idx = 7 - (x % 8);
        framebuffer[byte_idx_top] &= ~(1 << bit_idx);

        /* Bottom border */
        int byte_idx_bottom = ((DISPLAY_HEIGHT - 1) * DISPLAY_WIDTH + x) / 8;
        framebuffer[byte_idx_bottom] &= ~(1 << bit_idx);
    }

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        /* Left border */
        int byte_idx_left = (y * DISPLAY_WIDTH) / 8;
        framebuffer[byte_idx_left] &= ~(1 << 7);

        /* Right border */
        int byte_idx_right = (y * DISPLAY_WIDTH + DISPLAY_WIDTH - 1) / 8;
        framebuffer[byte_idx_right] &= ~(1 << 0);
    }

    /* Draw diagonal line */
    for (int i = 0; i < DISPLAY_WIDTH && i < DISPLAY_HEIGHT; i++) {
        int byte_idx = (i * DISPLAY_WIDTH + i) / 8;
        int bit_idx = 7 - (i % 8);
        framebuffer[byte_idx] &= ~(1 << bit_idx);
    }

    LOG_INF("Test pattern drawn to framebuffer");
}

int main(void)
{
    LOG_INF("===========================================");
    LOG_INF("  GxEPD2 Basic Example for Zephyr RTOS");
    LOG_INF("===========================================");

#if GXEPD2_DT_AVAILABLE

    uint16_t width, height;
    const char *panel_type;

    /* Initialize display from device tree */
    if (!gxepd2_init_from_dt(NULL)) {
        LOG_ERR("Failed to initialize GxEPD2 from device tree");
        return -1;
    }

    /* Get display properties */
    if (!gxepd2_get_dt_properties(NULL, &width, &height, &panel_type)) {
        LOG_ERR("Failed to get display properties");
        return -1;
    }

    LOG_INF("Display initialized:");
    LOG_INF("  Panel Type: %s", panel_type);
    LOG_INF("  Width: %d pixels", width);
    LOG_INF("  Height: %d pixels", height);

    /* Draw test pattern */
    draw_test_pattern();

    LOG_INF("Example complete!");
    LOG_INF("");
    LOG_INF("Next steps:");
    LOG_INF("  1. Connect your e-paper display according to the device tree overlay");
    LOG_INF("  2. To use a specific display driver, port it from src/epd/");
    LOG_INF("  3. Call display.writeImage() and display.refresh() to update the screen");
    LOG_INF("  4. Refer to the original Arduino examples for more usage patterns");

#else
    LOG_ERR("No e-paper display configured in device tree!");
    LOG_ERR("Please copy the appropriate overlay to your project:");
    LOG_ERR("  - nrf52840dk_nrf52840.overlay");
    LOG_ERR("  - nrf5340dk_nrf5340_cpuapp.overlay");
    LOG_ERR("  - nrf54l15dk_nrf54l15_cpuapp.overlay");
    return -1;
#endif

    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
