# Quick Start Guide - GxEPD2 on Zephyr RTOS

Get your e-paper display running on Zephyr in under 10 minutes!

## Prerequisites

- Zephyr SDK 4.2.1 or later installed
- West tool installed
- One of the supported Nordic development kits:
  - nRF52840 DK
  - nRF5340 DK
  - nRF54L15 DK
- Good Display or Waveshare e-paper display

## Step 1: Hardware Setup

### Wiring for nRF52840DK / nRF5340DK

```
Display Pin → Development Kit Pin
─────────────────────────────────
VCC         → 3.3V
GND         → GND
DIN (MOSI)  → P1.02
CLK (SCK)   → P1.01
CS          → P1.03
DC          → P1.04
RST         → P1.05
BUSY        → P1.06
```

### Wiring for nRF54L15DK

```
Display Pin → Development Kit Pin
─────────────────────────────────
VCC         → 3.3V
GND         → GND
DIN (MOSI)  → P1.05
CLK (SCK)   → P1.04
CS          → P1.06
DC          → P1.07
RST         → P1.08
BUSY        → P1.09
```

**⚠️ Important:** E-paper displays require **3.3V** - do NOT use 5V!

## Step 2: Run the Sample Application

### For nRF52840 DK:

```bash
cd /path/to/GxEPD2/zephyr/samples/gxepd2_basic
west build -b nrf52840dk_nrf52840 -p
west flash
```

### For nRF5340 DK:

```bash
cd /path/to/GxEPD2/zephyr/samples/gxepd2_basic
west build -b nrf5340dk_nrf5340_cpuapp -p
west flash
```

### For nRF54L15 DK:

```bash
cd /path/to/GxEPD2/zephyr/samples/gxepd2_basic
west build -b nrf54l15dk_nrf54l15_cpuapp -p
west flash
```

## Step 3: View Output

Connect to the serial console:

```bash
# Linux/macOS
west attach

# Or use screen
screen /dev/ttyACM0 115200

# Windows (use your COM port)
putty -serial COM3 -sercfg 115200,8,n,1,N
```

You should see:

```
*** Booting Zephyr OS build v4.2.1 ***
[00:00:00.000] <inf> main: ===========================================
[00:00:00.000] <inf> main:   GxEPD2 Basic Example for Zephyr RTOS
[00:00:00.000] <inf> main: ===========================================
[00:00:00.001] <inf> gxepd2_zephyr: GxEPD2 initialized from device tree
[00:00:00.001] <inf> main: Display initialized: GDEP015OC1 (200x200)
```

## Step 4: Customize for Your Display

Edit the device tree overlay for your board (copy it to your project first):

```dts
epaper: epaper@0 {
    compatible = "gooddisplay,gxepd2";
    reg = <0>;
    spi-max-frequency = <4000000>;
    dc-gpios = <&gpio1 4 GPIO_ACTIVE_HIGH>;
    reset-gpios = <&gpio1 5 GPIO_ACTIVE_HIGH>;
    busy-gpios = <&gpio1 6 GPIO_ACTIVE_HIGH>;

    /* CHANGE THESE FOR YOUR DISPLAY */
    width = <200>;                    /* Your display width */
    height = <200>;                   /* Your display height */
    panel-type = "GDEP015OC1";       /* Your panel model */
};
```

Common panel types:
- `GDEP015OC1` - 1.54" 200x200
- `GDEW0213T5D` - 2.13" 250x122
- `GDEW029T5` - 2.9" 296x128
- `GDEW042T2` - 4.2" 400x300
- `GDEW075T8` - 7.5" 800x480

## Step 5: Use in Your Own Project

### Option A: Add GxEPD2 as a Module

Add to your `west.yml`:

```yaml
manifest:
  projects:
    - name: GxEPD2
      url: https://github.com/ZinggJM/GxEPD2
      revision: main
      path: modules/lib/GxEPD2
```

Run:
```bash
west update
```

### Option B: Copy Files Directly

Copy the zephyr folder to your project:

```bash
cp -r /path/to/GxEPD2/zephyr /your/project/gxepd2
```

Add to your `CMakeLists.txt`:

```cmake
add_subdirectory(gxepd2)
```

### Enable in prj.conf

```ini
CONFIG_GXEPD2=y
CONFIG_SPI=y
CONFIG_GPIO=y
CONFIG_CPP=y
```

### Copy Device Tree Overlay

```bash
# For your board (example: nRF52840DK)
cp boards/nrf52840dk_nrf52840.overlay /your/project/
```

### Minimal Application Code

```cpp
#include <zephyr/kernel.h>
#include "gxepd2_zephyr.h"

int main(void)
{
    /* Initialize display */
    gxepd2_init_from_dt(NULL);

    /* Your code here */

    return 0;
}
```

## Troubleshooting

### Problem: "SPI device not ready"
**Solution:** Check that SPI pins are correctly defined in device tree overlay

### Problem: "Display not responding"
**Solution:**
1. Verify 3.3V power supply
2. Check all wire connections
3. Ensure BUSY pin is connected

### Problem: Build fails with "undefined reference"
**Solution:** Make sure `CONFIG_GXEPD2=y` is in prj.conf

### Problem: "Failed to initialize GPIO"
**Solution:** Verify GPIO pins in device tree overlay match your board

## Next Steps

1. **Port a specific driver:** See main README.md for instructions
2. **Draw graphics:** Implement drawing functions using framebuffer
3. **Add more displays:** Create additional device tree overlays
4. **Optimize performance:** Adjust SPI frequency and refresh modes

## Getting Help

- Read the full documentation: `zephyr/README.md`
- Check the sample application: `zephyr/samples/gxepd2_basic/`
- Review device tree bindings: `zephyr/dts/bindings/`
- Visit original library: https://github.com/ZinggJM/GxEPD2

## Summary Checklist

- [ ] Hardware properly wired (3.3V!)
- [ ] Correct board selected in build command
- [ ] Device tree overlay copied to project
- [ ] prj.conf has CONFIG_GXEPD2=y
- [ ] Panel type matches your display
- [ ] Width and height are correct
- [ ] Sample application builds and runs

**Congratulations!** You now have GxEPD2 running on Zephyr RTOS! 🎉
