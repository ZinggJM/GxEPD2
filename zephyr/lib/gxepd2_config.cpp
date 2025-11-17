/**
 * @file gxepd2_config.cpp
 * @brief Auto-configuration - supports ALL 37 display drivers!
 */

#include "gxepd2_config.h"
#include "gxepd2_zephyr.h"

// Include ALL original display drivers from src/epd/
#ifdef CONFIG_GXEPD2_102
#include "epd/GxEPD2_102.h"
#endif
#ifdef CONFIG_GXEPD2_150_BN
#include "epd/GxEPD2_150_BN.h"
#endif
#ifdef CONFIG_GXEPD2_154
#include "epd/GxEPD2_154.h"
#endif
#ifdef CONFIG_GXEPD2_154_D67
#include "epd/GxEPD2_154_D67.h"
#endif
#ifdef CONFIG_GXEPD2_154_M09
#include "epd/GxEPD2_154_M09.h"
#endif
#ifdef CONFIG_GXEPD2_154_M10
#include "epd/GxEPD2_154_M10.h"
#endif
#ifdef CONFIG_GXEPD2_154_T8
#include "epd/GxEPD2_154_T8.h"
#endif
#ifdef CONFIG_GXEPD2_213
#include "epd/GxEPD2_213.h"
#endif
#ifdef CONFIG_GXEPD2_213_B72
#include "epd/GxEPD2_213_B72.h"
#endif
#ifdef CONFIG_GXEPD2_213_B73
#include "epd/GxEPD2_213_B73.h"
#endif
#ifdef CONFIG_GXEPD2_213_B74
#include "epd/GxEPD2_213_B74.h"
#endif
#ifdef CONFIG_GXEPD2_213_BN
#include "epd/GxEPD2_213_BN.h"
#endif
#ifdef CONFIG_GXEPD2_213_M21
#include "epd/GxEPD2_213_M21.h"
#endif
#ifdef CONFIG_GXEPD2_213_T5D
#include "epd/GxEPD2_213_T5D.h"
#endif
#ifdef CONFIG_GXEPD2_213_flex
#include "epd/GxEPD2_213_flex.h"
#endif
#ifdef CONFIG_GXEPD2_260
#include "epd/GxEPD2_260.h"
#endif
#ifdef CONFIG_GXEPD2_260_M01
#include "epd/GxEPD2_260_M01.h"
#endif
#ifdef CONFIG_GXEPD2_266_BN
#include "epd/GxEPD2_266_BN.h"
#endif
#ifdef CONFIG_GXEPD2_270
#include "epd/GxEPD2_270.h"
#endif
#ifdef CONFIG_GXEPD2_290
#include "epd/GxEPD2_290.h"
#endif
#ifdef CONFIG_GXEPD2_290_T5
#include "epd/GxEPD2_290_T5.h"
#endif
#ifdef CONFIG_GXEPD2_290_T5D
#include "epd/GxEPD2_290_T5D.h"
#endif
#ifdef CONFIG_GXEPD2_290_T94
#include "epd/GxEPD2_290_T94.h"
#endif
#ifdef CONFIG_GXEPD2_290_T94_V2
#include "epd/GxEPD2_290_T94_V2.h"
#endif
#ifdef CONFIG_GXEPD2_290_M06
#include "epd/GxEPD2_290_M06.h"
#endif
#ifdef CONFIG_GXEPD2_290_BS
#include "epd/GxEPD2_290_BS.h"
#endif
#ifdef CONFIG_GXEPD2_371
#include "epd/GxEPD2_371.h"
#endif
#ifdef CONFIG_GXEPD2_420
#include "epd/GxEPD2_420.h"
#endif
#ifdef CONFIG_GXEPD2_420_M01
#include "epd/GxEPD2_420_M01.h"
#endif
#ifdef CONFIG_GXEPD2_420_M02
#include "epd/GxEPD2_420_M02.h"
#endif
#ifdef CONFIG_GXEPD2_437
#include "epd/GxEPD2_437.h"
#endif
#ifdef CONFIG_GXEPD2_565
#include "epd/GxEPD2_565.h"
#endif
#ifdef CONFIG_GXEPD2_583
#include "epd/GxEPD2_583.h"
#endif
#ifdef CONFIG_GXEPD2_583_T8
#include "epd/GxEPD2_583_T8.h"
#endif
#ifdef CONFIG_GXEPD2_750
#include "epd/GxEPD2_750.h"
#endif
#ifdef CONFIG_GXEPD2_750_T7
#include "epd/GxEPD2_750_T7.h"
#endif
#ifdef CONFIG_GXEPD2_1160_T91
#include "epd/GxEPD2_1160_T91.h"
#endif
#ifdef CONFIG_GXEPD2_1248
#include "epd/GxEPD2_1248.h"
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

    /* Initialize HAL from device tree */
    if (!gxepd2_init_from_dt(NULL)) {
        LOG_ERR("Failed to initialize HAL from device tree");
        return -1;
    }

    /* Create display driver instance based on Kconfig selection */
    /* All 37 drivers supported! */

#ifdef CONFIG_GXEPD2_102
    g_display = new GxEPD2_102(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_102 driver (1.02\" 128x80)");
#elif defined(CONFIG_GXEPD2_150_BN)
    g_display = new GxEPD2_150_BN(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_150_BN driver (1.50\" 200x200)");
#elif defined(CONFIG_GXEPD2_154)
    g_display = new GxEPD2_154(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_154 driver (1.54\" 200x200)");
#elif defined(CONFIG_GXEPD2_154_D67)
    g_display = new GxEPD2_154_D67(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_154_D67 driver (1.54\" 200x200)");
#elif defined(CONFIG_GXEPD2_154_M09)
    g_display = new GxEPD2_154_M09(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_154_M09 driver (1.54\" 200x200)");
#elif defined(CONFIG_GXEPD2_154_M10)
    g_display = new GxEPD2_154_M10(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_154_M10 driver (1.54\" 200x200)");
#elif defined(CONFIG_GXEPD2_154_T8)
    g_display = new GxEPD2_154_T8(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_154_T8 driver (1.54\" 200x200)");
#elif defined(CONFIG_GXEPD2_213)
    g_display = new GxEPD2_213(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213 driver (2.13\" 250x122)");
#elif defined(CONFIG_GXEPD2_213_B72)
    g_display = new GxEPD2_213_B72(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213_B72 driver (2.13\" 250x122)");
#elif defined(CONFIG_GXEPD2_213_B73)
    g_display = new GxEPD2_213_B73(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213_B73 driver (2.13\" 250x122)");
#elif defined(CONFIG_GXEPD2_213_B74)
    g_display = new GxEPD2_213_B74(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213_B74 driver (2.13\" 250x122)");
#elif defined(CONFIG_GXEPD2_213_BN)
    g_display = new GxEPD2_213_BN(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213_BN driver (2.13\" 250x122)");
#elif defined(CONFIG_GXEPD2_213_M21)
    g_display = new GxEPD2_213_M21(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213_M21 driver (2.13\" 250x122)");
#elif defined(CONFIG_GXEPD2_213_T5D)
    g_display = new GxEPD2_213_T5D(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213_T5D driver (2.13\" 250x122)");
#elif defined(CONFIG_GXEPD2_213_flex)
    g_display = new GxEPD2_213_flex(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_213_flex driver (2.13\" Flexible)");
#elif defined(CONFIG_GXEPD2_260)
    g_display = new GxEPD2_260(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_260 driver (2.6\" 296x152)");
#elif defined(CONFIG_GXEPD2_260_M01)
    g_display = new GxEPD2_260_M01(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_260_M01 driver (2.6\" 296x152)");
#elif defined(CONFIG_GXEPD2_266_BN)
    g_display = new GxEPD2_266_BN(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_266_BN driver (2.66\" 296x152)");
#elif defined(CONFIG_GXEPD2_270)
    g_display = new GxEPD2_270(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_270 driver (2.7\" 264x176)");
#elif defined(CONFIG_GXEPD2_290)
    g_display = new GxEPD2_290(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_290 driver (2.9\" 296x128)");
#elif defined(CONFIG_GXEPD2_290_T5)
    g_display = new GxEPD2_290_T5(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_290_T5 driver (2.9\" 296x128)");
#elif defined(CONFIG_GXEPD2_290_T5D)
    g_display = new GxEPD2_290_T5D(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_290_T5D driver (2.9\" 296x128)");
#elif defined(CONFIG_GXEPD2_290_T94)
    g_display = new GxEPD2_290_T94(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_290_T94 driver (2.9\" 296x128)");
#elif defined(CONFIG_GXEPD2_290_T94_V2)
    g_display = new GxEPD2_290_T94_V2(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_290_T94_V2 driver (2.9\" 296x128)");
#elif defined(CONFIG_GXEPD2_290_M06)
    g_display = new GxEPD2_290_M06(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_290_M06 driver (2.9\" 296x128)");
#elif defined(CONFIG_GXEPD2_290_BS)
    g_display = new GxEPD2_290_BS(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_290_BS driver (2.9\" 296x128)");
#elif defined(CONFIG_GXEPD2_371)
    g_display = new GxEPD2_371(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_371 driver (3.7\" 416x240)");
#elif defined(CONFIG_GXEPD2_420)
    g_display = new GxEPD2_420(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_420 driver (4.2\" 400x300)");
#elif defined(CONFIG_GXEPD2_420_M01)
    g_display = new GxEPD2_420_M01(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_420_M01 driver (4.2\" 400x300)");
#elif defined(CONFIG_GXEPD2_420_M02)
    g_display = new GxEPD2_420_M02(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_420_M02 driver (4.2\" 400x300)");
#elif defined(CONFIG_GXEPD2_437)
    g_display = new GxEPD2_437(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_437 driver (4.37\" 512x368)");
#elif defined(CONFIG_GXEPD2_565)
    g_display = new GxEPD2_565(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_565 driver (5.65\" 600x448)");
#elif defined(CONFIG_GXEPD2_583)
    g_display = new GxEPD2_583(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_583 driver (5.83\" 648x480)");
#elif defined(CONFIG_GXEPD2_583_T8)
    g_display = new GxEPD2_583_T8(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_583_T8 driver (5.83\" 648x480)");
#elif defined(CONFIG_GXEPD2_750)
    g_display = new GxEPD2_750(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_750 driver (7.5\" 800x480)");
#elif defined(CONFIG_GXEPD2_750_T7)
    g_display = new GxEPD2_750_T7(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_750_T7 driver (7.5\" 800x480)");
#elif defined(CONFIG_GXEPD2_1160_T91)
    g_display = new GxEPD2_1160_T91(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_1160_T91 driver (11.6\" 1304x984)");
#elif defined(CONFIG_GXEPD2_1248)
    g_display = new GxEPD2_1248(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);
    LOG_INF("Created GxEPD2_1248 driver (12.48\" 1304x984)");
#else
    LOG_ERR("No display driver configured! Select one in prj.conf");
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
        if (width) *width = 0;
        if (height) *height = 0;
    }
}

void gxepd2_print_config(void)
{
    printk("\n");
    printk("╔════════════════════════════════════════════════════════════╗\n");
    printk("║           GxEPD2 E-Paper Display Configuration            ║\n");
    printk("╠════════════════════════════════════════════════════════════╣\n");

    if (g_display) {
        uint16_t w = g_display->WIDTH;
        uint16_t h = g_display->HEIGHT;
        printk("║ Resolution    : %4d x %-4d pixels                       ║\n", w, h);
    }

    printk("╠════════════════════════════════════════════════════════════╣\n");
    printk("║ SPI Frequency : %d MHz                                  ║\n", CONFIG_GXEPD2_SPI_FREQUENCY / 1000000);
    printk("║ Auto-Init     : %s                                        ║\n",
           IS_ENABLED(CONFIG_GXEPD2_AUTO_INIT) ? "Enabled " : "Disabled");
    printk("║ Status        : %s                                   ║\n",
           g_display ? "Initialized" : "Not Initialized");
    printk("╚════════════════════════════════════════════════════════════╝\n");
    printk("\n");
}
