# GxEPD2 Simple Example - Zero Device Tree Editing Required!

This is the **simplest possible** way to use GxEPD2 on Zephyr. No device tree editing required!

## Quick Start (3 Steps)

### Step 1: Edit prj.conf

Open `prj.conf` and select your display:

```ini
# Uncomment your display model:
CONFIG_GXEPD2_DISPLAY_154=y      # 1.54" (200x200)
# CONFIG_GXEPD2_DISPLAY_213=y    # 2.13" (250x122)
# CONFIG_GXEPD2_DISPLAY_290=y    # 2.9" (296x128)
# CONFIG_GXEPD2_DISPLAY_420=y    # 4.2" (400x300)
```

That's it! The library auto-detects your board and configures everything.

### Step 2: Build

```bash
# For nRF52840DK
west build -b nrf52840dk_nrf52840

# For nRF5340DK
west build -b nrf5340dk_nrf5340_cpuapp

# For nRF54L15DK
west build -b nrf54l15dk_nrf54l15_cpuapp
```

### Step 3: Flash and Run

```bash
west flash
```

## Wiring

The library automatically uses these default pins:

### nRF52840DK / nRF5340DK

| Display Pin | Board Pin |
|-------------|-----------|
| VCC         | 3.3V      |
| GND         | GND       |
| DIN (MOSI)  | P1.02     |
| CLK (SCK)   | P1.01     |
| CS          | P1.03     |
| DC          | P1.04     |
| RST         | P1.05     |
| BUSY        | P1.06     |

### nRF54L15DK

| Display Pin | Board Pin |
|-------------|-----------|
| VCC         | 3.3V      |
| GND         | GND       |
| DIN (MOSI)  | P1.05     |
| CLK (SCK)   | P1.04     |
| CS          | P1.06     |
| DC          | P1.07     |
| RST         | P1.08     |
| BUSY        | P1.09     |

## Using the Display in Your Code

Super simple - just 3 lines:

```cpp
#include "gxepd2_config.h"

int main(void) {
    // Get display (auto-initialized!)
    GxEPD2_EPD *display = gxepd2_get_display();

    // Clear screen
    display->clearScreen();

    // Done!
    display->powerOff();
}
```

## Configuration Options

All in `prj.conf`:

```ini
# Display selection (required)
CONFIG_GXEPD2_DISPLAY_154=y

# Board selection (optional - auto-detects by default)
CONFIG_GXEPD2_BOARD_AUTO=y

# Auto-initialization (recommended)
CONFIG_GXEPD2_AUTO_INIT=y

# SPI speed (optional - default is 4 MHz)
CONFIG_GXEPD2_SPI_FREQUENCY=4000000

# Logging level
CONFIG_GXEPD2_LOG_LEVEL_INF=y
```

## Customizing Pins

If you want to use different pins:

1. Set `CONFIG_GXEPD2_BOARD_CUSTOM=y` in `prj.conf`
2. Copy the appropriate overlay from `zephyr/boards/` to your project
3. Edit the pin assignments in the overlay

## Example Output

```
========================================
  GxEPD2 Simple Example
========================================

╔════════════════════════════════════════════════════════════╗
║           GxEPD2 E-Paper Display Configuration            ║
╠════════════════════════════════════════════════════════════╣
║ Display Model : 1.54" GDEP015OC1                           ║
║ Board         : nRF52840/5340 DK                           ║
║ Resolution    : 200 x 200 pixels                           ║
╠════════════════════════════════════════════════════════════╣
║ Pin Configuration:                                         ║
║   SPI1: SCK=P1.01 MOSI=P1.02, CS=P1.03, DC=P1.04...       ║
╠════════════════════════════════════════════════════════════╣
║ SPI Frequency : 4 MHz                                      ║
║ Auto-Init     : Enabled                                    ║
║ Status        : Initialized                                ║
╚════════════════════════════════════════════════════════════╝

SUCCESS! Display is ready to use.
```

## Troubleshooting

**"Display not initialized"**
- Make sure `CONFIG_GXEPD2_AUTO_INIT=y` in prj.conf
- Check that you selected a display model

**"Board auto-detection failed"**
- Manually set board: `CONFIG_GXEPD2_BOARD_NRF52840DK=y`

**Build errors**
- Ensure you're using a supported board
- Check that CONFIG_SPI=y and CONFIG_GPIO=y are enabled

## Available Displays

Current status:
- ✅ 1.54" (200x200) - `CONFIG_GXEPD2_DISPLAY_154=y` - **READY**
- ⏳ 2.13" (250x122) - Coming soon
- ⏳ 2.9" (296x128) - Coming soon
- ⏳ 4.2" (400x300) - Coming soon

More displays will be added! For now, use the 1.54" display or port your own driver.

## That's It!

No device tree editing. No complex configuration. Just select your display and build!
