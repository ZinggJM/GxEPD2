/**
 * @file gxepd2_zephyr.cpp
 * @brief Zephyr-specific helpers for GxEPD2 library implementation
 */

#include "gxepd2_zephyr.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gxepd2_zephyr, CONFIG_GXEPD2_LOG_LEVEL);

bool gxepd2_init_from_dt(const struct device *epaper_node)
{
#if DT_NODE_EXISTS(DT_NODELABEL(epaper))

    /* Get SPI device */
    const struct device *spi_dev = DEVICE_DT_GET(DT_BUS(DT_NODELABEL(epaper)));
    if (!device_is_ready(spi_dev)) {
        LOG_ERR("SPI device not ready");
        return false;
    }

    /* Configure SPI */
    struct spi_config spi_cfg = {
        .frequency = CONFIG_GXEPD2_SPI_FREQUENCY,
        .operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8) | SPI_MODE_CPOL | SPI_MODE_CPHA,
        .slave = 0,
        .cs = {
            .gpio = GPIO_DT_SPEC_GET(DT_NODELABEL(epaper), cs_gpios),
            .delay = 0,
        },
    };

    /* Initialize SPI in the global SPI object */
    if (!SPI.init(spi_dev, &spi_cfg)) {
        LOG_ERR("Failed to initialize SPI");
        return false;
    }

    /* Initialize GPIO pins */
    struct gpio_dt_spec cs_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(epaper), cs_gpios);
    struct gpio_dt_spec dc_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(epaper), dc_gpios);
    struct gpio_dt_spec rst_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(epaper), reset_gpios);
    struct gpio_dt_spec busy_spec = GPIO_DT_SPEC_GET(DT_NODELABEL(epaper), busy_gpios);

    /* Register GPIO pins with the HAL */
    if (!zephyr_gpio_init_pin(GXEPD2_PIN_CS, &cs_spec)) {
        LOG_ERR("Failed to initialize CS pin");
        return false;
    }

    if (!zephyr_gpio_init_pin(GXEPD2_PIN_DC, &dc_spec)) {
        LOG_ERR("Failed to initialize DC pin");
        return false;
    }

    if (!zephyr_gpio_init_pin(GXEPD2_PIN_RST, &rst_spec)) {
        LOG_ERR("Failed to initialize RST pin");
        return false;
    }

    if (!zephyr_gpio_init_pin(GXEPD2_PIN_BUSY, &busy_spec)) {
        LOG_ERR("Failed to initialize BUSY pin");
        return false;
    }

    LOG_INF("GxEPD2 initialized from device tree");
    LOG_INF("  Width: %d", DT_PROP(DT_NODELABEL(epaper), width));
    LOG_INF("  Height: %d", DT_PROP(DT_NODELABEL(epaper), height));
    LOG_INF("  Panel: %s", DT_PROP(DT_NODELABEL(epaper), panel_type));

    return true;

#else
    LOG_ERR("No epaper node found in device tree");
    return false;
#endif
}

bool gxepd2_get_dt_properties(const struct device *epaper_node,
                               uint16_t *width, uint16_t *height,
                               const char **panel_type)
{
#if DT_NODE_EXISTS(DT_NODELABEL(epaper))

    if (width) {
        *width = DT_PROP(DT_NODELABEL(epaper), width);
    }

    if (height) {
        *height = DT_PROP(DT_NODELABEL(epaper), height);
    }

    if (panel_type) {
        *panel_type = DT_PROP(DT_NODELABEL(epaper), panel_type);
    }

    return true;

#else
    return false;
#endif
}
