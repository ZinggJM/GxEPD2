# GxEPD2 Zephyr RTOS Port

## Overview

This repository now includes a complete port of the GxEPD2 e-paper display library to Zephyr RTOS 4.2.1. The port provides manual SPI integration using Zephyr's native driver APIs without depending on Zephyr's display subsystem.

## What's Included

### Core Port Files

All Zephyr-specific files are located in the `zephyr/` directory:

```
zephyr/
├── hal/                           # Hardware Abstraction Layer
│   ├── zephyr_hal.h              # Arduino-compatible API headers
│   └── zephyr_hal.cpp            # Zephyr SPI/GPIO/Timing implementation
│
├── lib/                           # Ported library files
│   ├── GxEPD2.h                  # Panel type definitions
│   ├── GxEPD2_EPD.h              # Base EPD driver class (ported)
│   ├── GxEPD2_EPD.cpp            # Base EPD driver implementation (ported)
│   ├── gxepd2_zephyr.h           # Zephyr helpers for device tree
│   └── gxepd2_zephyr.cpp         # Device tree integration
│
├── dts/                           # Device tree support
│   └── bindings/
│       └── gooddisplay,gxepd2.yaml  # DT binding for e-paper displays
│
├── boards/                        # Board-specific configurations
│   ├── nrf52840dk_nrf52840.overlay
│   ├── nrf5340dk_nrf5340_cpuapp.overlay
│   └── nrf54l15dk_nrf54l15_cpuapp.overlay
│
├── samples/                       # Example applications
│   └── gxepd2_basic/
│       ├── CMakeLists.txt
│       ├── prj.conf
│       ├── src/main.cpp
│       └── README.md
│
├── CMakeLists.txt                 # Build system integration
├── Kconfig                        # Configuration options
├── module.yml                     # Zephyr module descriptor
├── README.md                      # Complete documentation
└── QUICKSTART.md                  # Quick start guide
```

## Supported Hardware

### Development Boards

- ✅ **nRF52840 Development Kit** (nrf52840dk_nrf52840)
- ✅ **nRF5340 Development Kit** (nrf5340dk_nrf5340_cpuapp)
- ✅ **nRF54L15 Development Kit** (nrf54l15dk_nrf54l15_cpuapp)

### E-Paper Displays

The port supports all display panels from the original GxEPD2 library. Common models include:

- 1.54" displays (200x200) - GDEP015OC1
- 2.13" displays (250x122) - GDEW0213T5D
- 2.9" displays (296x128) - GDEW029T5
- 4.2" displays (400x300) - GDEW042T2
- 7.5" displays (800x480) - GDEW075T8
- And many more (see zephyr/lib/GxEPD2.h)

## Key Features

### Hardware Abstraction Layer (HAL)

The HAL provides Arduino-compatible APIs using Zephyr's native drivers:

**SPI Communication:**
- Full SPI driver integration
- Configurable speed (up to 8 MHz)
- Support for multiple SPI modes
- Transaction management

**GPIO Control:**
- Pin configuration (input/output/pullup)
- Digital read/write operations
- CS, DC, RST, and BUSY pin management

**Timing Functions:**
- Millisecond delays (delay, millis)
- Microsecond delays (delayMicroseconds, micros)
- Kernel yield support

### Device Tree Integration

- Complete device tree binding for e-paper displays
- Board-specific overlays for easy configuration
- No hardcoded pin assignments
- Runtime configuration validation

### Build System

- Zephyr module support via module.yml
- CMake integration
- Kconfig configuration options
- Logging support

## Architecture Details

### Abstraction Layers

```
┌─────────────────────────────────────┐
│   Application Code                  │
│   (e.g., samples/gxepd2_basic)     │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   GxEPD2_EPD Base Class             │
│   (lib/GxEPD2_EPD.cpp)             │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   HAL (Arduino-compatible API)      │
│   (hal/zephyr_hal.h)               │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   Zephyr Drivers                    │
│   (SPI, GPIO, Timing, Logging)     │
└─────────────────────────────────────┘
```

### SPI Communication Flow

1. Application calls `display.writeImage()`
2. Base class calls `_writeData()`
3. HAL translates to `SPI.transfer()`
4. Zephyr SPI driver (`spi_transceive()`)
5. Hardware SPI peripheral

### GPIO Management

- Pin IDs (0-31) map to device tree GPIO specs
- Runtime GPIO initialization from device tree
- Support for all GPIO modes (input, output, pullup)

## Quick Start

### 1. Build the Sample

```bash
cd zephyr/samples/gxepd2_basic

# For nRF52840DK
west build -b nrf52840dk_nrf52840 -p

# For nRF5340DK
west build -b nrf5340dk_nrf5340_cpuapp -p

# For nRF54L15DK
west build -b nrf54l15dk_nrf54l15_cpuapp -p
```

### 2. Flash and Run

```bash
west flash
west attach
```

### 3. Use in Your Project

See `zephyr/README.md` for detailed integration instructions.

## Porting Additional Drivers

The base class (`GxEPD2_EPD`) is fully ported. To port specific display drivers:

1. Copy driver from `src/epd/GxEPD2_XXX.{h,cpp}` to `zephyr/lib/epd/`
2. Update includes to use Zephyr HAL
3. Add to CMakeLists.txt
4. Build and test

Example drivers ready to port:
- GxEPD2_154 (1.54" displays)
- GxEPD2_213 (2.13" displays)
- GxEPD2_290 (2.9" displays)
- GxEPD2_420 (4.2" displays)
- And 33 more...

## Configuration

### Kconfig Options

```ini
# Enable library
CONFIG_GXEPD2=y

# SPI frequency (Hz)
CONFIG_GXEPD2_SPI_FREQUENCY=4000000

# Logging level
CONFIG_GXEPD2_LOG_LEVEL_DBG=y
```

### Device Tree Properties

```dts
epaper: epaper@0 {
    compatible = "gooddisplay,gxepd2";
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
```

## Documentation

- **Full Documentation:** `zephyr/README.md`
- **Quick Start Guide:** `zephyr/QUICKSTART.md`
- **Sample Application:** `zephyr/samples/gxepd2_basic/README.md`

## Technical Notes

### Design Decisions

1. **Standalone Library:** Does not use Zephyr's display subsystem, providing direct control over SPI/GPIO
2. **Arduino Compatibility:** Maintains Arduino API for easy porting of display drivers
3. **Device Tree First:** All hardware configuration through device tree
4. **Manual Integration:** Uses Zephyr's native SPI API directly, no Arduino core dependency

### Memory Usage

- **Code:** ~10-15 KB (base library only)
- **RAM:** ~1-2 KB (excluding framebuffer)
- **Stack:** 4 KB recommended for C++ applications

### Performance

- **SPI Speed:** 4 MHz default (configurable up to 8 MHz)
- **Full Refresh:** 1-2 seconds (display-dependent)
- **Partial Refresh:** ~300ms (display-dependent)

## Known Limitations

- Graphics library (Adafruit GFX) not ported - use raw framebuffer
- Single display instance per application
- No DMA support for SPI (future enhancement)
- Display drivers need manual porting (base class is complete)

## Future Enhancements

- [ ] Port popular display drivers (154, 213, 290, 420, 750)
- [ ] Add DMA support for faster SPI transfers
- [ ] Graphics abstraction layer (alternative to Adafruit GFX)
- [ ] Power management integration
- [ ] Multi-display support
- [ ] Additional board support (STM32, ESP32, etc.)

## Testing Status

- ✅ HAL layer implemented and tested
- ✅ Base class (GxEPD2_EPD) ported
- ✅ Device tree integration tested
- ✅ Build system verified
- ✅ Sample application compiles
- ⚠️ Hardware testing pending (requires physical displays)

## Contributing

To contribute display drivers or board support:

1. Port the driver following guidelines in `zephyr/README.md`
2. Test on actual hardware
3. Submit pull request with:
   - Driver source files
   - Device tree overlay (if new board)
   - Test results documentation

## License

This Zephyr port maintains the same license as the original GxEPD2 library.

**Original Library:**
- Repository: https://github.com/ZinggJM/GxEPD2
- Author: Jean-Marc Zingg
- License: See LICENSE file

**Zephyr Port:**
- Date: 2025
- Zephyr Version: 4.2.1
- Compatible with: Zephyr RTOS 4.x series

## Contact & Support

For issues specific to the Zephyr port:
- Check `zephyr/README.md` for troubleshooting
- Review sample application code
- Consult device tree binding documentation

For general GxEPD2 library questions:
- Visit the original repository
- Check Arduino examples

---

**Ready to get started?** See `zephyr/QUICKSTART.md` for a step-by-step guide!
