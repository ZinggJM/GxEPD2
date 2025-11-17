# Getting Started with GxEPD2 on Zephyr - Quick Reference

## 🚀 Fastest Way to Get Started (60 seconds!)

### 1. Hardware Setup

Connect your 1.54" e-paper display to your nRF52840DK, nRF5340DK, or nRF54L15DK:

```
Display → Nordic DK
─────────────────────
VCC   → 3.3V
GND   → GND
DIN   → P1.02  (nRF52/53) or P1.05 (nRF54L15)
CLK   → P1.01  (nRF52/53) or P1.04 (nRF54L15)
CS    → P1.03  (nRF52/53) or P1.06 (nRF54L15)
DC    → P1.04  (nRF52/53) or P1.07 (nRF54L15)
RST   → P1.05  (nRF52/53) or P1.08 (nRF54L15)
BUSY  → P1.06  (nRF52/53) or P1.09 (nRF54L15)
```

### 2. Try the Sample

```bash
cd GxEPD2/zephyr/samples/gxepd2_simple

# Build for your board
west build -b nrf52840dk_nrf52840 -p    # OR
west build -b nrf5340dk_nrf5340_cpuapp -p    # OR
west build -b nrf54l15dk_nrf54l15_cpuapp -p

# Flash
west flash

# View output
west attach
```

You should see the configuration printed and "SUCCESS! Display is ready to use."

### 3. Use in Your Project

Add to your **prj.conf**:
```ini
CONFIG_GXEPD2=y
CONFIG_GXEPD2_DISPLAY_154=y
CONFIG_GXEPD2_AUTO_INIT=y
CONFIG_SPI=y
CONFIG_GPIO=y
CONFIG_CPP=y
```

In your **main.cpp**:
```cpp
#include "gxepd2_config.h"

int main(void) {
    GxEPD2_EPD *display = gxepd2_get_display();
    display->clearScreen();
    // Draw your content here
    display->powerOff();
    return 0;
}
```

Build:
```bash
west build -b nrf52840dk_nrf52840
west flash
```

## 📖 Next Steps

| What you want | Where to look |
|---------------|---------------|
| Simple setup guide | `zephyr/SIMPLE_SETUP.md` |
| Complete documentation | `zephyr/README.md` |
| Traditional setup | `zephyr/QUICKSTART.md` |
| API reference | See includes in `zephyr/lib/` |
| Sample code | `zephyr/samples/gxepd2_simple/` |

## 🔧 Configuration Options

### Display Selection (Choose ONE)

```ini
CONFIG_GXEPD2_DISPLAY_154=y    # 1.54" 200x200 ✅ Available
CONFIG_GXEPD2_DISPLAY_213=y    # 2.13" 250x122 ⏳ Coming Soon
CONFIG_GXEPD2_DISPLAY_290=y    # 2.9" 296x128 ⏳ Coming Soon
CONFIG_GXEPD2_DISPLAY_420=y    # 4.2" 400x300 ⏳ Coming Soon
```

### Board Selection

```ini
CONFIG_GXEPD2_BOARD_AUTO=y         # Auto-detect (recommended)
CONFIG_GXEPD2_BOARD_NRF52840DK=y   # Manual select nRF52840
CONFIG_GXEPD2_BOARD_NRF5340DK=y    # Manual select nRF5340
CONFIG_GXEPD2_BOARD_NRF54L15DK=y   # Manual select nRF54L15
CONFIG_GXEPD2_BOARD_CUSTOM=y       # Use device tree overlay
```

### Advanced Options

```ini
CONFIG_GXEPD2_AUTO_INIT=y              # Auto-initialize at boot
CONFIG_GXEPD2_SPI_FREQUENCY=4000000    # SPI speed (Hz)
CONFIG_GXEPD2_LOG_LEVEL_DBG=y          # Debug logging
```

## 💻 Code Examples

### Minimal Example
```cpp
#include "gxepd2_config.h"

int main(void) {
    auto display = gxepd2_get_display();
    display->clearScreen();
    display->powerOff();
}
```

### Print Configuration
```cpp
#include "gxepd2_config.h"

int main(void) {
    gxepd2_print_config();  // Shows board, display, pins, etc.

    GxEPD2_EPD *display = gxepd2_get_display();
    // Use display...
}
```

### Get Display Info
```cpp
#include "gxepd2_config.h"

int main(void) {
    uint16_t width, height;
    gxepd2_get_dimensions(&width, &height);
    printk("Display: %dx%d\n", width, height);
}
```

### Drawing Example
```cpp
#include "gxepd2_config.h"
#include <stdlib.h>

int main(void) {
    auto display = gxepd2_get_display();

    size_t size = (display->WIDTH * display->HEIGHT) / 8;
    uint8_t *fb = (uint8_t*)malloc(size);

    memset(fb, 0xFF, size);  // Clear to white
    // Draw to framebuffer...

    display->writeImage(fb, 0, 0, display->WIDTH, display->HEIGHT);
    display->refresh();
    display->powerOff();

    free(fb);
}
```

## 🔍 API Reference

### Initialization Functions
```cpp
int gxepd2_init(void);                      // Manual init
GxEPD2_EPD* gxepd2_get_display(void);      // Get display instance
void gxepd2_get_dimensions(uint16_t *w, uint16_t *h);
void gxepd2_print_config(void);            // Print config info
```

### Display Functions
```cpp
display->init();                            // Initialize hardware
display->clearScreen(0xFF);                 // Clear to white
display->writeImage(buf, x, y, w, h);      // Write image data
display->refresh();                         // Update display
display->powerOff();                        // Power down
```

## 🐛 Troubleshooting

| Problem | Solution |
|---------|----------|
| "Display not initialized" | Add `CONFIG_GXEPD2_AUTO_INIT=y` to prj.conf |
| "No display driver configured" | Add `CONFIG_GXEPD2_DISPLAY_154=y` to prj.conf |
| Display not responding | Check 3.3V power, verify wiring, check BUSY pin |
| Build errors | Ensure `CONFIG_SPI=y`, `CONFIG_GPIO=y`, `CONFIG_CPP=y` |
| Board auto-detect failed | Manually set: `CONFIG_GXEPD2_BOARD_NRF52840DK=y` |

## 📁 Project Structure

```
zephyr/
├── SIMPLE_SETUP.md              ← Start here for simple setup
├── QUICKSTART.md                ← Traditional device tree setup
├── README.md                    ← Complete documentation
├── GETTING_STARTED.md           ← This file
│
├── samples/
│   ├── gxepd2_simple/          ← Simplest example (3+3 lines)
│   └── gxepd2_basic/           ← Traditional example
│
├── lib/
│   ├── gxepd2_config.h         ← Auto-config API
│   ├── GxEPD2_EPD.h           ← Base display driver
│   └── epd/
│       └── GxEPD2_154.h       ← 1.54" display driver
│
└── boards/                      ← Device tree overlays (optional)
```

## 🎯 What to Read First

**Just want it to work?**
→ Try the sample: `zephyr/samples/gxepd2_simple/`

**New to Zephyr?**
→ Read: `zephyr/SIMPLE_SETUP.md`

**Want full documentation?**
→ Read: `zephyr/README.md`

**Need custom pins?**
→ Read: `zephyr/QUICKSTART.md`

## ✅ Checklist

Before asking for help, verify:

- [ ] Using a supported board (nRF52840/5340/54L15 DK)
- [ ] Display model selected in prj.conf
- [ ] CONFIG_GXEPD2_AUTO_INIT=y enabled
- [ ] CONFIG_SPI=y and CONFIG_GPIO=y enabled
- [ ] 3.3V power to display (NOT 5V!)
- [ ] All pins wired according to board defaults
- [ ] BUSY pin connected
- [ ] Build completes without errors

## 🎉 Success Checklist

You know it's working when:

- [ ] Build completes successfully
- [ ] Flash succeeds
- [ ] Console shows "GxEPD2 initialized successfully!"
- [ ] Console shows "SUCCESS! Display is ready to use."
- [ ] Display configuration table is printed
- [ ] No error messages in log

## 🚀 Where to Go From Here

1. **Make it work:** Follow this guide
2. **Understand it:** Read `SIMPLE_SETUP.md`
3. **Customize it:** Read `README.md`
4. **Port more displays:** See display driver examples in `src/epd/`
5. **Advanced features:** Read `QUICKSTART.md` and `README.md`

---

**Need more help?** Check the documentation files listed above!

**Found a bug?** Report it at: https://github.com/ZinggJM/GxEPD2/issues

**Happy coding!** 🎨
