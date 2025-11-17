/**
 * @file zephyr_hal.cpp
 * @brief Hardware Abstraction Layer implementation for GxEPD2 on Zephyr RTOS
 */

#include "zephyr_hal.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gxepd2_hal, CONFIG_GXEPD2_LOG_LEVEL);

/* Global instances */
SerialClass Serial;
SPIClass SPI;

/* GPIO Pin storage - support up to 32 pins */
#define MAX_GPIO_PINS 32
static struct zephyr_gpio_pin gpio_pins[MAX_GPIO_PINS];

/* ============================================================================
 * SPIClass Implementation
 * ============================================================================ */

SPIClass::SPIClass() : _spi_dev(NULL), _in_transaction(false), _current_freq(4000000)
{
    memset(&_spi_cfg, 0, sizeof(_spi_cfg));
    memset(&_tx_buf, 0, sizeof(_tx_buf));
    memset(&_rx_buf, 0, sizeof(_rx_buf));

    _tx_bufs.buffers = &_tx_buf;
    _tx_bufs.count = 1;
    _rx_bufs.buffers = &_rx_buf;
    _rx_bufs.count = 1;
}

bool SPIClass::init(const struct device *spi_dev, const struct spi_config *config)
{
    if (!device_is_ready(spi_dev)) {
        LOG_ERR("SPI device not ready");
        return false;
    }

    _spi_dev = spi_dev;
    memcpy(&_spi_cfg, config, sizeof(struct spi_config));

    LOG_INF("SPI initialized successfully");
    return true;
}

void SPIClass::begin()
{
    /* SPI initialization is done via init() with device tree */
    if (_spi_dev != NULL) {
        LOG_DBG("SPI begin");
    }
}

void SPIClass::end()
{
    _in_transaction = false;
    LOG_DBG("SPI end");
}

void SPIClass::beginTransaction(struct SPISettings settings)
{
    if (_spi_dev == NULL) {
        LOG_ERR("SPI not initialized");
        return;
    }

    /* Update SPI frequency if changed */
    if (settings.clock != _current_freq) {
        _spi_cfg.frequency = settings.clock;
        _current_freq = settings.clock;
    }

    /* Update SPI mode */
    _spi_cfg.operation &= ~(SPI_MODE_CPOL | SPI_MODE_CPHA);
    if (settings.dataMode & 0x02) {
        _spi_cfg.operation |= SPI_MODE_CPOL;
    }
    if (settings.dataMode & 0x01) {
        _spi_cfg.operation |= SPI_MODE_CPHA;
    }

    /* Update bit order */
    _spi_cfg.operation &= ~SPI_TRANSFER_LSB;
    if (settings.bitOrder == LSBFIRST) {
        _spi_cfg.operation |= SPI_TRANSFER_LSB;
    }

    _in_transaction = true;
}

void SPIClass::endTransaction()
{
    _in_transaction = false;
}

uint8_t SPIClass::transfer(uint8_t data)
{
    if (_spi_dev == NULL) {
        LOG_ERR("SPI not initialized");
        return 0;
    }

    uint8_t rx_data = 0;

    _tx_buf.buf = &data;
    _tx_buf.len = 1;
    _rx_buf.buf = &rx_data;
    _rx_buf.len = 1;

    int ret = spi_transceive(_spi_dev, &_spi_cfg, &_tx_bufs, &_rx_bufs);
    if (ret < 0) {
        LOG_ERR("SPI transfer failed: %d", ret);
        return 0;
    }

    return rx_data;
}

void SPIClass::transfer(void *buf, size_t count)
{
    if (_spi_dev == NULL) {
        LOG_ERR("SPI not initialized");
        return;
    }

    _tx_buf.buf = buf;
    _tx_buf.len = count;
    _rx_buf.buf = NULL;
    _rx_buf.len = 0;

    int ret = spi_write(_spi_dev, &_spi_cfg, &_tx_bufs);
    if (ret < 0) {
        LOG_ERR("SPI write failed: %d", ret);
    }
}

/* ============================================================================
 * GPIO Functions
 * ============================================================================ */

bool zephyr_gpio_init_pin(int16_t pin_id, const struct gpio_dt_spec *gpio_spec)
{
    if (pin_id < 0 || pin_id >= MAX_GPIO_PINS) {
        LOG_ERR("Invalid pin ID: %d", pin_id);
        return false;
    }

    if (!device_is_ready(gpio_spec->port)) {
        LOG_ERR("GPIO device not ready for pin %d", pin_id);
        return false;
    }

    gpio_pins[pin_id].port = gpio_spec->port;
    gpio_pins[pin_id].pin = gpio_spec->pin;
    gpio_pins[pin_id].flags = gpio_spec->dt_flags;
    gpio_pins[pin_id].initialized = true;

    LOG_DBG("Initialized GPIO pin %d", pin_id);
    return true;
}

struct zephyr_gpio_pin* zephyr_gpio_get_pin(int16_t pin_id)
{
    if (pin_id < 0 || pin_id >= MAX_GPIO_PINS) {
        return NULL;
    }

    if (!gpio_pins[pin_id].initialized) {
        return NULL;
    }

    return &gpio_pins[pin_id];
}

void pinMode(int16_t pin, uint8_t mode)
{
    struct zephyr_gpio_pin *gpio_pin = zephyr_gpio_get_pin(pin);
    if (gpio_pin == NULL) {
        return;
    }

    gpio_flags_t flags;

    switch (mode) {
        case OUTPUT:
            flags = GPIO_OUTPUT_INACTIVE;
            break;
        case INPUT:
            flags = GPIO_INPUT;
            break;
        case INPUT_PULLUP:
            flags = GPIO_INPUT | GPIO_PULL_UP;
            break;
        default:
            flags = GPIO_INPUT;
            break;
    }

    gpio_pin_configure(gpio_pin->port, gpio_pin->pin, flags);
}

void digitalWrite(int16_t pin, uint8_t value)
{
    struct zephyr_gpio_pin *gpio_pin = zephyr_gpio_get_pin(pin);
    if (gpio_pin == NULL) {
        return;
    }

    gpio_pin_set(gpio_pin->port, gpio_pin->pin, value);
}

int digitalRead(int16_t pin)
{
    struct zephyr_gpio_pin *gpio_pin = zephyr_gpio_get_pin(pin);
    if (gpio_pin == NULL) {
        return 0;
    }

    return gpio_pin_get(gpio_pin->port, gpio_pin->pin);
}

/* ============================================================================
 * Timing Functions
 * ============================================================================ */

void delay(uint32_t ms)
{
    k_msleep(ms);
}

void delayMicroseconds(uint32_t us)
{
    k_usleep(us);
}

uint32_t millis(void)
{
    return k_uptime_get_32();
}

uint32_t micros(void)
{
    return k_cyc_to_us_floor64(k_cycle_get_32());
}

/* ============================================================================
 * SerialClass Implementation (Debug Output)
 * ============================================================================ */

void SerialClass::begin(uint32_t baud)
{
    /* Logging is initialized by Zephyr - nothing to do */
    (void)baud;
}

void SerialClass::print(const char* str)
{
    LOG_INF("%s", str);
}

void SerialClass::print(uint32_t val)
{
    LOG_INF("%u", val);
}

void SerialClass::println(const char* str)
{
    LOG_INF("%s", str);
}

void SerialClass::println(uint32_t val)
{
    LOG_INF("%u", val);
}
