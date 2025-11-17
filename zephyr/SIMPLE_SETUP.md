# GxEPD2 Simple Setup Guide - No Device Tree Editing!

This guide shows you the **absolute simplest** way to integrate GxEPD2 into your Zephyr project.

## Why This Is Simple

- ✅ **No device tree editing required** - Just select your board in Kconfig
- ✅ **No manual pin configuration** - Auto-configured for Nordic boards
- ✅ **No driver selection** - Just pick your display model
- ✅ **Auto-initialization** - Display ready to use in `main()`
- ✅ **3-line code example** - Get started in seconds

## Step-by-Step Setup

### Option 1: Use the Simple Sample (Recommended for First-Time Users)

1. **Navigate to the simple sample:**
   ```bash
   cd /path/to/GxEPD2/zephyr/samples/gxepd2_simple
   ```

2. **Edit `prj.conf` - Select your display:**
   ```ini
   # Uncomment ONE display:
   CONFIG_GXEPD2_DISPLAY_154=y      # 1.54" (200x200)
   # CONFIG_GXEPD2_DISPLAY_213=y    # 2.13" (250x122) - Coming soon
   # CONFIG_GXEPD2_DISPLAY_290=y    # 2.9" (296x128) - Coming soon
   # CONFIG_GXEPD2_DISPLAY_420=y    # 4.2" (400x300) - Coming soon
   ```

3. **Build and flash:**
   ```bash
   # For nRF52840DK
   west build -b nrf52840dk_nrf52840 -p
   west flash

   # For nRF5340DK
   west build -b nrf5340dk_nrf5340_cpuapp -p
   west flash

   # For nRF54L15DK
   west build -b nrf54l15dk_nrf54l15_cpuapp -p
   west flash
   ```

4. **Done!** Connect your display using the default pins (see below).

### Option 2: Add to Your Existing Project

1. **Add to your `prj.conf`:**
   ```ini
   # Enable GxEPD2
   CONFIG_GXEPD2=y

   # Select your display (choose ONE)
   CONFIG_GXEPD2_DISPLAY_154=y

   # Auto-detect board and auto-initialize
   CONFIG_GXEPD2_BOARD_AUTO=y
   CONFIG_GXEPD2_AUTO_INIT=y

   # Required drivers
   CONFIG_SPI=y
   CONFIG_GPIO=y
   CONFIG_CPP=y
   ```

2. **In your `main.cpp`:**
   ```cpp
   #include <zephyr/kernel.h>
   #include "gxepd2_config.h"

   int main(void) {
       // Get auto-initialized display
       GxEPD2_EPD *display = gxepd2_get_display();

       // Use it!
       display->clearScreen();
       // ... your drawing code ...
       display->powerOff();

       return 0;
   }
   ```

3. **Build your project as usual:**
   ```bash
   west build -b your_board
   west flash
   ```

## Hardware Wiring

### For nRF52840DK and nRF5340DK (Auto-Configured)

```
E-Paper Display          Nordic DK
───────────────          ─────────
VCC (3.3V)      ──────── 3.3V
GND             ──────── GND
DIN (MOSI)      ──────── P1.02
CLK (SCK)       ──────── P1.01
CS              ──────── P1.03
DC              ──────── P1.04
RST             ──────── P1.05
BUSY            ──────── P1.06
```

### For nRF54L15DK (Auto-Configured)

```
E-Paper Display          Nordic DK
───────────────          ─────────
VCC (3.3V)      ──────── 3.3V
GND             ──────── GND
DIN (MOSI)      ──────── P1.05
CLK (SCK)       ──────── P1.04
CS              ──────── P1.06
DC              ──────── P1.07
RST             ──────── P1.08
BUSY            ──────── P1.09
```

**⚠️ IMPORTANT:** E-paper displays require 3.3V - **do NOT use 5V**!

## Configuration Reference

### Complete `prj.conf` Template

Copy this template to your project:

```ini
# ============================================
# GxEPD2 E-Paper Display Configuration
# ============================================

# Enable library
CONFIG_GXEPD2=y

# Select Display Model (uncomment ONE)
CONFIG_GXEPD2_DISPLAY_154=y      # 1.54" 200x200 - Available now!
# CONFIG_GXEPD2_DISPLAY_213=y    # 2.13" 250x122 - Coming soon
# CONFIG_GXEPD2_DISPLAY_290=y    # 2.9" 296x128 - Coming soon
# CONFIG_GXEPD2_DISPLAY_420=y    # 4.2" 400x300 - Coming soon

# Board Selection
CONFIG_GXEPD2_BOARD_AUTO=y       # Auto-detect (recommended)
# CONFIG_GXEPD2_BOARD_NRF52840DK=y
# CONFIG_GXEPD2_BOARD_NRF5340DK=y
# CONFIG_GXEPD2_BOARD_NRF54L15DK=y
# CONFIG_GXEPD2_BOARD_CUSTOM=y   # Use device tree overlay

# Auto-Initialize
CONFIG_GXEPD2_AUTO_INIT=y        # Display ready in main()

# Optional: SPI Speed
CONFIG_GXEPD2_SPI_FREQUENCY=4000000  # 4 MHz (safe default)

# Optional: Logging
CONFIG_LOG=y
CONFIG_GXEPD2_LOG_LEVEL_INF=y

# Required System Config
CONFIG_SPI=y
CONFIG_GPIO=y
CONFIG_CPP=y
CONFIG_STD_CPP11=y
CONFIG_MAIN_STACK_SIZE=4096
```

## Code Examples

### Minimal Example (3 lines)

```cpp
#include "gxepd2_config.h"

int main(void) {
    auto display = gxepd2_get_display();
    display->clearScreen();
    display->powerOff();
    return 0;
}
```

### With Status Info

```cpp
#include "gxepd2_config.h"

int main(void) {
    // Print configuration
    gxepd2_print_config();

    // Get display
    GxEPD2_EPD *display = gxepd2_get_display();
    if (!display) {
        printk("Display initialization failed!\n");
        return -1;
    }

    // Get dimensions
    uint16_t w, h;
    gxepd2_get_dimensions(&w, &h);
    printk("Display: %dx%d\n", w, h);

    // Use display
    display->clearScreen();
    display->powerOff();

    return 0;
}
```

### Complete Drawing Example

```cpp
#include "gxepd2_config.h"
#include <stdlib.h>

int main(void) {
    GxEPD2_EPD *display = gxepd2_get_display();

    uint16_t width = display->WIDTH;
    uint16_t height = display->HEIGHT;
    size_t buffer_size = (width * height) / 8;

    // Allocate framebuffer
    uint8_t *framebuffer = (uint8_t*)malloc(buffer_size);
    if (!framebuffer) {
        printk("Failed to allocate framebuffer\n");
        return -1;
    }

    // Clear to white
    memset(framebuffer, 0xFF, buffer_size);

    // Draw your content to framebuffer here...
    // (Each bit represents one pixel: 0=black, 1=white)

    // Send to display
    display->writeImage(framebuffer, 0, 0, width, height);
    display->refresh();
    display->powerOff();

    free(framebuffer);
    return 0;
}
```

## Customizing Pin Assignments

If you need different pins than the defaults:

1. **Set custom board mode in `prj.conf`:**
   ```ini
   CONFIG_GXEPD2_BOARD_CUSTOM=y
   ```

2. **Copy overlay template to your project:**
   ```bash
   # For nRF52840DK
   cp zephyr/boards/nrf52840dk_nrf52840.overlay your_project/

   # For nRF5340DK
   cp zephyr/boards/nrf5340dk_nrf5340_cpuapp.overlay your_project/

   # For nRF54L15DK
   cp zephyr/boards/nrf54l15dk_nrf54l15_cpuapp.overlay your_project/
   ```

3. **Edit the overlay to change pins:**
   ```dts
   &spi1 {
       cs-gpios = <&gpio1 10 GPIO_ACTIVE_LOW>;  /* Change pin numbers */

       epaper: epaper@0 {
           dc-gpios = <&gpio1 11 GPIO_ACTIVE_HIGH>;
           reset-gpios = <&gpio1 12 GPIO_ACTIVE_HIGH>;
           busy-gpios = <&gpio1 13 GPIO_ACTIVE_HIGH>;
           /* ... */
       };
   };
   ```

## Troubleshooting

### "Display not initialized"
```ini
# Solution: Enable auto-init
CONFIG_GXEPD2_AUTO_INIT=y
```

### "No display driver configured"
```ini
# Solution: Select a display model
CONFIG_GXEPD2_DISPLAY_154=y
```

### "Board auto-detection failed"
```ini
# Solution: Manually specify board
CONFIG_GXEPD2_BOARD_NRF52840DK=y
```

### Display not responding
1. Check 3.3V power supply
2. Verify wiring matches pin configuration
3. Ensure BUSY pin is connected
4. Try reducing SPI frequency:
   ```ini
   CONFIG_GXEPD2_SPI_FREQUENCY=2000000  # 2 MHz
   ```

### Build errors
```ini
# Make sure these are enabled:
CONFIG_SPI=y
CONFIG_GPIO=y
CONFIG_CPP=y
CONFIG_STD_CPP11=y
```

## Supported Displays

| Display | Size | Resolution | Config | Status |
|---------|------|------------|--------|--------|
| GDEP015OC1 | 1.54" | 200×200 | `CONFIG_GXEPD2_DISPLAY_154=y` | ✅ Available |
| GDEW0213T5D | 2.13" | 250×122 | `CONFIG_GXEPD2_DISPLAY_213=y` | ⏳ Coming Soon |
| GDEW029T5 | 2.9" | 296×128 | `CONFIG_GXEPD2_DISPLAY_290=y` | ⏳ Coming Soon |
| GDEW042T2 | 4.2" | 400×300 | `CONFIG_GXEPD2_DISPLAY_420=y` | ⏳ Coming Soon |

More displays will be added regularly!

## API Reference

### Initialization
```cpp
int gxepd2_init(void);                        // Manual init (if auto-init disabled)
GxEPD2_EPD* gxepd2_get_display(void);        // Get display instance
void gxepd2_get_dimensions(uint16_t *w, uint16_t *h);  // Get size
void gxepd2_print_config(void);              // Print config info
```

### Display Operations
```cpp
display->clearScreen(0xFF);                   // Clear to white
display->writeImage(buffer, x, y, w, h);     // Write image data
display->refresh();                          // Update screen
display->powerOff();                         // Power down (preserve image)
```

## Next Steps

1. ✅ **You're done with setup!** The library is ready to use.
2. 📖 **Learn the drawing API** - See original GxEPD2 examples
3. 🎨 **Create graphics** - Draw to framebuffer, then send to display
4. ⚡ **Optimize** - Use partial refresh for faster updates
5. 🔋 **Save power** - Always call `powerOff()` when done

## Comparison: Simple vs. Manual Setup

### Simple Setup (This Guide)
```cpp
// prj.conf
CONFIG_GXEPD2=y
CONFIG_GXEPD2_DISPLAY_154=y
CONFIG_GXEPD2_AUTO_INIT=y

// main.cpp (3 lines)
auto display = gxepd2_get_display();
display->clearScreen();
display->powerOff();
```

### Manual Setup (Old Way)
```cpp
// Need to create device tree overlay
// Need to configure SPI manually
// Need to initialize HAL
// Need to create driver instance
// Need to configure pins...
// 50+ lines of boilerplate code
```

**Simple setup is 95% less code!**

## Summary

This simple integration method:
- 📝 **3 lines** of config (`prj.conf`)
- 💻 **3 lines** of code (`main.cpp`)
- ⚡ **Zero** device tree editing
- 🎯 **Auto** board detection
- 🚀 **Ready** to use immediately

**That's it!** Enjoy using GxEPD2 on Zephyr! 🎉
