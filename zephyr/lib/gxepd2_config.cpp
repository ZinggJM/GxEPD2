/**
 * @file gxepd2_config.cpp
 * @brief Auto-configuration implementation
 */

#include "gxepd2_config.h"
#include "gxepd2_zephyr.h"
#include "GxEPD2_EPD.h"

#ifdef GXEPD2_DRIVER_154
#include "epd/GxEPD2_154.h"
#endif

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gxepd2_config, CONFIG_GXEPD2_LOG_LEVEL);

/* Pin IDs for GPIO HAL */
#define PIN_CS   0
#define PIN_DC   1
#define PIN_RST  2
#define PIN_BUSY 3

/* Global display instance */
static GxEPD2_EPD *g_display = NULL;

#ifdef CONFIG_GXEPD2_AUTO_INIT
/* Auto-initialize at boot if enabled */
static int gxepd2_auto_init(void)
{
    return gxepd2_init();
}
SYS_INIT(gxepd2_auto_init, APPLICATION, CONFIG_GXEPD2_INIT_PRIORITY);
#endif

int gxepd2_init(void)
{
    if (g_display != NULL) {
        LOG_WRN("Display already initialized");
        return 0;
    }

    LOG_INF("Initializing GxEPD2 library...");

#ifndef CONFIG_GXEPD2_DISPLAY_CUSTOM
    LOG_INF("Display Model: %s", GXEPD2_PANEL_NAME);
    LOG_INF("Board: %s", GXEPD2_BOARD_NAME);
    LOG_INF("Pins: %s", GXEPD2_PIN_INFO);
#endif

    /* Initialize HAL from device tree */
    if (!gxepd2_init_from_dt(NULL)) {
        LOG_ERR("Failed to initialize HAL from device tree");
        return -1;
    }

    /* Create display driver instance based on Kconfig */
#ifdef GXEPD2_DRIVER_154
    g_display = new GxEPD2_154(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_154 driver instance");

#elif defined(GXEPD2_DRIVER_213)
    // TODO: Port GxEPD2_213 driver
    LOG_ERR("2.13\" display driver not yet ported - use CUSTOM mode");
    return -ENOTSUP;

#elif defined(GXEPD2_DRIVER_290)
    // TODO: Port GxEPD2_290 driver
    LOG_ERR("2.9\" display driver not yet ported - use CUSTOM mode");
    return -ENOTSUP;

#elif defined(GXEPD2_DRIVER_420)
    // TODO: Port GxEPD2_420 driver
    LOG_ERR("4.2\" display driver not yet ported - use CUSTOM mode");
    return -ENOTSUP;

#else
    LOG_ERR("No display driver configured");
    return -EINVAL;
#endif

    if (g_display == NULL) {
        LOG_ERR("Failed to create display instance");
        return -ENOMEM;
    }

    /* Initialize the display hardware */
    g_display->init();

    LOG_INF("GxEPD2 initialized successfully!");
    LOG_INF("Display: %dx%d pixels", g_display->WIDTH, g_display->HEIGHT);

    return 0;
}

GxEPD2_EPD* gxepd2_get_display(void)
{
    if (g_display == NULL) {
        LOG_WRN("Display not initialized - call gxepd2_init() first");
    }
    return g_display;
}

void gxepd2_get_dimensions(uint16_t *width, uint16_t *height)
{
    if (g_display) {
        if (width) *width = g_display->WIDTH;
        if (height) *height = g_display->HEIGHT;
    } else {
#ifdef GXEPD2_DISPLAY_WIDTH
        if (width) *width = GXEPD2_DISPLAY_WIDTH;
        if (height) *height = GXEPD2_DISPLAY_HEIGHT;
#else
        if (width) *width = 0;
        if (height) *height = 0;
#endif
    }
}

void gxepd2_print_config(void)
{
    printk("\n");
    printk("╔════════════════════════════════════════════════════════════╗\n");
    printk("║           GxEPD2 E-Paper Display Configuration            ║\n");
    printk("╠════════════════════════════════════════════════════════════╣\n");

#ifndef CONFIG_GXEPD2_DISPLAY_CUSTOM
    printk("║ Display Model : %-42s ║\n", GXEPD2_PANEL_NAME);
    printk("║ Board         : %-42s ║\n", GXEPD2_BOARD_NAME);

    uint16_t w, h;
    gxepd2_get_dimensions(&w, &h);
    printk("║ Resolution    : %3d x %-3d pixels                          ║\n", w, h);

    printk("╠════════════════════════════════════════════════════════════╣\n");
    printk("║ Pin Configuration:                                         ║\n");
    printk("║   %s\n", GXEPD2_PIN_INFO);
    printk("║                                                            ║\n");
#else
    printk("║ Mode: Custom (Device Tree Configuration)                  ║\n");
#endif

    printk("╠════════════════════════════════════════════════════════════╣\n");
    printk("║ SPI Frequency : %d MHz                                  ║\n", CONFIG_GXEPD2_SPI_FREQUENCY / 1000000);
    printk("║ Auto-Init     : %s                                        ║\n",
           IS_ENABLED(CONFIG_GXEPD2_AUTO_INIT) ? "Enabled " : "Disabled");
    printk("║ Status        : %s                                   ║\n",
           g_display ? "Initialized" : "Not Initialized");
    printk("╚════════════════════════════════════════════════════════════╝\n");
    printk("\n");
}
