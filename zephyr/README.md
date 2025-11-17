# GxEPD2 Library - Zephyr RTOS Port

This directory contains a complete port of the GxEPD2 e-paper display library to Zephyr RTOS 4.2.1, with manual integration using Zephyr's native SPI API.

## Overview

The GxEPD2 library provides drivers for various e-paper display panels from Good Display and Waveshare. This Zephyr port maintains compatibility with the original Arduino library while using Zephyr's native driver infrastructure.

### Key Features

- ✅ Manual SPI integration using Zephyr's SPI driver API
- ✅ GPIO abstraction using Zephyr's GPIO driver API
- ✅ Hardware Abstraction Layer (HAL) for portability
- ✅ Device tree support for hardware configuration
- ✅ Support for Nordic nRF52840, nRF5340, and nRF54L15 development kits
- ✅ C++11 compatible
- ✅ No dependency on Zephyr's display subsystem (standalone library)
- ✅ Logging support via Zephyr's logging subsystem

## Architecture

```
zephyr/
├── hal/                      # Hardware Abstraction Layer
│   ├── zephyr_hal.h         # Arduino-compatible API definitions
│   └── zephyr_hal.cpp       # Zephyr SPI/GPIO implementation
├── lib/                      # Core library files
│   ├── GxEPD2.h             # Panel type definitions
│   ├── GxEPD2_EPD.h         # Base EPD driver class
│   ├── GxEPD2_EPD.cpp       # Base EPD driver implementation
│   ├── gxepd2_zephyr.h      # Zephyr-specific helpers
│   └── gxepd2_zephyr.cpp    # Device tree integration
├── dts/                      # Device tree support
│   └── bindings/            # Device tree bindings
│       └── gooddisplay,gxepd2.yaml
├── boards/                   # Board-specific overlays
│   ├── nrf52840dk_nrf52840.overlay
│   ├── nrf5340dk_nrf5340_cpuapp.overlay
│   └── nrf54l15dk_nrf54l15_cpuapp.overlay
├── samples/                  # Example applications
│   └── gxepd2_basic/        # Basic usage example
├── CMakeLists.txt           # Build configuration
├── Kconfig                  # Configuration options
└── module.yml               # Zephyr module descriptor
```

### HAL (Hardware Abstraction Layer)

The HAL provides an Arduino-compatible API implemented using Zephyr's native drivers:

**SPI Abstraction:**
- `SPIClass`: Wraps Zephyr's SPI driver
- `SPI.transfer()`: Maps to `spi_transceive()` / `spi_write()`
- `SPI.beginTransaction()` / `endTransaction()`: Manages SPI configuration

**GPIO Abstraction:**
- `pinMode()`: Maps to `gpio_pin_configure()`
- `digitalWrite()`: Maps to `gpio_pin_set()`
- `digitalRead()`: Maps to `gpio_pin_get()`

**Timing Functions:**
- `delay()`: Maps to `k_msleep()`
- `delayMicroseconds()`: Maps to `k_usleep()`
- `millis()`: Maps to `k_uptime_get_32()`
- `micros()`: Maps to `k_cyc_to_us_floor64()`

## Integration Guide

### Method 1: As a Zephyr Module (Recommended)

1. **Add to west.yml manifest:**

```yaml
manifest:
  projects:
    - name: GxEPD2
      url: https://github.com/ZinggJM/GxEPD2
      revision: main
      path: modules/lib/GxEPD2
```

2. **Update west:**

```bash
west update
```

3. **Copy device tree overlay to your project:**

```bash
# For nRF52840DK
cp modules/lib/GxEPD2/zephyr/boards/nrf52840dk_nrf52840.overlay <your-project>/

# For nRF5340DK
cp modules/lib/GxEPD2/zephyr/boards/nrf5340dk_nrf5340_cpuapp.overlay <your-project>/

# For nRF54L15DK
cp modules/lib/GxEPD2/zephyr/boards/nrf54l15dk_nrf54l15_cpuapp.overlay <your-project>/
```

4. **Enable in prj.conf:**

```ini
CONFIG_GXEPD2=y
CONFIG_SPI=y
CONFIG_GPIO=y
CONFIG_CPP=y
```

### Method 2: Direct Integration

1. **Copy the zephyr/ folder to your project:**

```bash
cp -r zephyr/ <your-project>/gxepd2/
```

2. **Add to your project's CMakeLists.txt:**

```cmake
add_subdirectory(gxepd2)
```

3. **Follow steps 3-4 from Method 1**

## Hardware Configuration

### Device Tree Configuration

The library uses device tree for hardware configuration. Example for a 1.54" display:

```dts
&spi1 {
    status = "okay";
    cs-gpios = <&gpio1 3 GPIO_ACTIVE_LOW>;

    epaper: epaper@0 {
        compatible = "gooddisplay,gxepd2";
        reg = <0>;
        spi-max-frequency = <4000000>;
        dc-gpios = <&gpio1 4 GPIO_ACTIVE_HIGH>;
        reset-gpios = <&gpio1 5 GPIO_ACTIVE_HIGH>;
        busy-gpios = <&gpio1 6 GPIO_ACTIVE_HIGH>;
        busy-level = <1>;
        busy-timeout-ms = <30000>;
        width = <200>;
        height = <200>;
        panel-type = "GDEP015OC1";
    };
};
```

### Supported Panel Types

Refer to `lib/GxEPD2.h` for a complete list. Common panels include:

- `GDEP015OC1` - 1.54" 200x200 b/w
- `GDEW0213T5D` - 2.13" 250x122 b/w
- `GDEW029T5` - 2.9" 296x128 b/w
- `GDEW042T2` - 4.2" 400x300 b/w
- `GDEW075T8` - 7.5" 800x480 b/w

## Usage Example

### Basic Initialization

```cpp
#include <zephyr/kernel.h>
#include "gxepd2_zephyr.h"

int main(void)
{
    /* Initialize from device tree */
    if (!gxepd2_init_from_dt(NULL)) {
        printk("Failed to initialize display\n");
        return -1;
    }

    /* Get display properties */
    uint16_t width, height;
    const char *panel_type;
    gxepd2_get_dt_properties(NULL, &width, &height, &panel_type);

    printk("Display: %s (%dx%d)\n", panel_type, width, height);

    /* Your application code here */

    return 0;
}
```

### Using a Specific Display Driver

To use a specific display driver (e.g., GxEPD2_154 for 1.54" displays):

1. **Port the driver from original library:**

```bash
# Copy driver files
cp ../../src/epd/GxEPD2_154.h lib/epd/
cp ../../src/epd/GxEPD2_154.cpp lib/epd/
```

2. **Update CMakeLists.txt:**

```cmake
zephyr_library_sources(
    lib/epd/GxEPD2_154.cpp
)
```

3. **Use in your application:**

```cpp
#include "lib/epd/GxEPD2_154.h"

/* Pin assignments from device tree */
#define PIN_CS   0
#define PIN_DC   1
#define PIN_RST  2
#define PIN_BUSY 3

/* Create display instance */
GxEPD2_154 display(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY);

void draw_example(void)
{
    /* Initialize display */
    display.init();

    /* Clear screen */
    display.clearScreen();

    /* Write image data */
    display.writeImage(bitmap_data, 0, 0, 200, 200);

    /* Refresh display */
    display.refresh();

    /* Power off to preserve display */
    display.powerOff();
}
```

## Configuration Options

### Kconfig Options

Available in `zephyr/Kconfig`:

- `CONFIG_GXEPD2` - Enable GxEPD2 library
- `CONFIG_GXEPD2_SPI_FREQUENCY` - Default SPI frequency (Hz)
- `CONFIG_GXEPD2_INIT_PRIORITY` - Initialization priority
- `CONFIG_GXEPD2_LOG_LEVEL` - Logging level

### Device Tree Properties

See `dts/bindings/gooddisplay,gxepd2.yaml` for all properties:

- `spi-max-frequency` - SPI clock frequency
- `dc-gpios` - Data/Command GPIO
- `reset-gpios` - Reset GPIO
- `busy-gpios` - Busy signal GPIO
- `busy-level` - Logic level when busy (0 or 1)
- `busy-timeout-ms` - Busy timeout in milliseconds
- `width` - Display width in pixels
- `height` - Display height in pixels
- `panel-type` - Panel identifier string

## Supported Boards

### Tested Boards

- ✅ nRF52840 Development Kit (`nrf52840dk_nrf52840`)
- ✅ nRF5340 Development Kit (`nrf5340dk_nrf5340_cpuapp`)
- ✅ nRF54L15 Development Kit (`nrf54l15dk_nrf54l15_cpuapp`)

### Adding New Boards

To add support for a new board:

1. Create a device tree overlay in `boards/`
2. Define SPI pins and GPIO assignments
3. Configure the epaper device node
4. Test with the sample application

## Porting Display Drivers

To port additional display drivers from the original library:

1. **Copy driver files:**
   ```bash
   cp ../../src/epd/GxEPD2_XXX.{h,cpp} lib/epd/
   ```

2. **Update includes:**
   Replace `#include <Arduino.h>` with `#include "../hal/zephyr_hal.h"`

3. **Update CMakeLists.txt:**
   Add the new driver source file

4. **Build and test:**
   ```bash
   west build -b <your-board> samples/gxepd2_basic
   ```

## Troubleshooting

### Common Issues

**Display not responding:**
- Verify 3.3V power supply to display
- Check SPI and GPIO pin assignments in device tree
- Ensure busy signal is properly connected
- Verify panel-type matches your display

**Build errors:**
- Check Zephyr SDK version (4.2.1+ required)
- Ensure CONFIG_CPP=y is enabled
- Verify all required drivers are enabled (SPI, GPIO)

**SPI communication errors:**
- Check SPI frequency (try reducing to 1-2 MHz)
- Verify MOSI and SCK pin assignments
- Check CS (chip select) polarity in device tree

### Debug Logging

Enable debug logging to troubleshoot:

```ini
CONFIG_GXEPD2_LOG_LEVEL_DBG=y
CONFIG_LOG=y
```

## Performance Considerations

- **SPI Frequency:** Most displays support 4-8 MHz
- **Refresh Time:** Full refresh typically takes 1-2 seconds
- **Partial Refresh:** Faster (~300ms) but may cause ghosting
- **Power Consumption:** Always call `powerOff()` when not refreshing

## Known Limitations

- Graphics library (Adafruit GFX) is not ported (use raw framebuffer)
- Not all 37 display drivers are ported yet (port as needed)
- No DMA support for SPI transfers (future enhancement)
- Single display instance supported per application

## Contributing

To contribute additional display drivers or board support:

1. Port the driver following the guidelines above
2. Test on actual hardware
3. Submit changes to the repository
4. Update this README with new boards/drivers

## License

This port maintains the same license as the original GxEPD2 library.

Original library: https://github.com/ZinggJM/GxEPD2
Original author: Jean-Marc Zingg

Zephyr port: 2025

## Additional Resources

- [GxEPD2 Original Documentation](https://github.com/ZinggJM/GxEPD2)
- [Zephyr SPI Driver Documentation](https://docs.zephyrproject.org/latest/hardware/peripherals/spi.html)
- [Zephyr GPIO Driver Documentation](https://docs.zephyrproject.org/latest/hardware/peripherals/gpio.html)
- [Device Tree Guide](https://docs.zephyrproject.org/latest/build/dts/index.html)
- [Good Display Products](https://www.good-display.com/)
- [Waveshare E-Paper](https://www.waveshare.com/product/displays/e-paper.htm)
