/**
 * @file zephyr_hal.h
 * @brief Hardware Abstraction Layer for GxEPD2 on Zephyr RTOS
 *
 * This HAL provides Arduino-compatible API using Zephyr's native drivers
 * for SPI, GPIO, and timing functions.
 *
 * Compatible with Zephyr 4.2.1
 */

#ifndef _ZEPHYR_HAL_H_
#define _ZEPHYR_HAL_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Arduino-compatible types */
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define MSBFIRST 0
#define LSBFIRST 1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

/* Pin definition structure for Zephyr GPIO */
struct zephyr_gpio_pin {
    const struct device *port;
    gpio_pin_t pin;
    gpio_flags_t flags;
    bool initialized;
};

/* SPI Settings structure (Arduino compatible) */
struct SPISettings {
    uint32_t clock;
    uint8_t bitOrder;
    uint8_t dataMode;
};

/* SPI Class wrapper for Zephyr */
class SPIClass {
public:
    SPIClass();
    void begin();
    void end();
    void beginTransaction(struct SPISettings settings);
    void endTransaction();
    uint8_t transfer(uint8_t data);
    void transfer(void *buf, size_t count);

    bool init(const struct device *spi_dev, const struct spi_config *config);

private:
    const struct device *_spi_dev;
    struct spi_config _spi_cfg;
    struct spi_buf _tx_buf;
    struct spi_buf _rx_buf;
    struct spi_buf_set _tx_bufs;
    struct spi_buf_set _rx_bufs;
    bool _in_transaction;
    uint32_t _current_freq;
};

/* GPIO Functions (Arduino compatible) */
void pinMode(int16_t pin, uint8_t mode);
void digitalWrite(int16_t pin, uint8_t value);
int digitalRead(int16_t pin);

/* Timing Functions (Arduino compatible) */
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
uint32_t millis(void);
uint32_t micros(void);

/* GPIO Pin Management */
bool zephyr_gpio_init_pin(int16_t pin_id, const struct gpio_dt_spec *gpio_spec);
struct zephyr_gpio_pin* zephyr_gpio_get_pin(int16_t pin_id);

/* Serial/Debug output stubs (for diagnostic messages) */
class SerialClass {
public:
    void begin(uint32_t baud);
    void print(const char* str);
    void print(uint32_t val);
    void println(const char* str);
    void println(uint32_t val);
};

extern SerialClass Serial;
extern SPIClass SPI;

/* PROGMEM support - on Zephyr, just use regular memory */
#define PROGMEM
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#define pgm_read_word(addr) (*(const uint16_t *)(addr))
#define pgm_read_dword(addr) (*(const uint32_t *)(addr))

#ifdef __cplusplus
}
#endif

#endif /* _ZEPHYR_HAL_H_ */
