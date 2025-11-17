# GxEPD2 Basic Sample Application

This sample demonstrates basic usage of the GxEPD2 e-paper display library on Zephyr RTOS.

## Hardware Requirements

- One of the following Nordic development kits:
  - nRF52840 DK
  - nRF5340 DK
  - nRF54L15 DK

- Good Display or Waveshare e-paper display (SPI interface)

## Wiring

### Default Pin Configuration (nRF52840DK / nRF5340DK)

| Display Pin | nRF Pin | Description |
|-------------|---------|-------------|
| VCC         | 3.3V    | Power supply |
| GND         | GND     | Ground |
| DIN (MOSI)  | P1.02   | SPI MOSI |
| CLK (SCK)   | P1.01   | SPI Clock |
| CS          | P1.03   | Chip Select |
| DC          | P1.04   | Data/Command |
| RST         | P1.05   | Reset |
| BUSY        | P1.06   | Busy signal |

### Default Pin Configuration (nRF54L15DK)

| Display Pin | nRF Pin | Description |
|-------------|---------|-------------|
| VCC         | 3.3V    | Power supply |
| GND         | GND     | Ground |
| DIN (MOSI)  | P1.05   | SPI MOSI |
| CLK (SCK)   | P1.04   | SPI Clock |
| CS          | P1.06   | Chip Select |
| DC          | P1.07   | Data/Command |
| RST         | P1.08   | Reset |
| BUSY        | P1.09   | Busy signal |

**Note:** Pin assignments can be customized by editing the device tree overlay files.

## Building and Flashing

### For nRF52840 DK:

```bash
cd zephyr/samples/gxepd2_basic
west build -b nrf52840dk_nrf52840
west flash
```

### For nRF5340 DK:

```bash
cd zephyr/samples/gxepd2_basic
west build -b nrf5340dk_nrf5340_cpuapp
west flash
```

### For nRF54L15 DK:

```bash
cd zephyr/samples/gxepd2_basic
west build -b nrf54l15dk_nrf54l15_cpuapp
west flash
```

## Customization

### Changing Display Type

Edit the device tree overlay file for your board (e.g., `nrf52840dk_nrf52840.overlay`) and modify:

```dts
panel-type = "GDEP015OC1";  /* Change to your display model */
width = <200>;               /* Change to your display width */
height = <200>;              /* Change to your display height */
```

Supported panel types are defined in `zephyr/lib/GxEPD2.h`.

### Adding a Specific Display Driver

To use a specific display driver (e.g., GxEPD2_154), you need to:

1. Port the driver from `src/epd/GxEPD2_154.cpp` to `zephyr/lib/epd/`
2. Update `zephyr/CMakeLists.txt` to include the driver
3. Include the driver header in your application
4. Instantiate the driver class instead of the base class

## Expected Output

```
*** Booting Zephyr OS build v4.2.1 ***
[00:00:00.000,000] <inf> main: ===========================================
[00:00:00.000,000] <inf> main:   GxEPD2 Basic Example for Zephyr RTOS
[00:00:00.000,000] <inf> main: ===========================================
[00:00:00.001,000] <inf> gxepd2_zephyr: GxEPD2 initialized from device tree
[00:00:00.001,000] <inf> gxepd2_zephyr:   Width: 200
[00:00:00.001,000] <inf> gxepd2_zephyr:   Height: 200
[00:00:00.001,000] <inf> gxepd2_zephyr:   Panel: GDEP015OC1
[00:00:00.001,000] <inf> main: Display initialized:
[00:00:00.001,000] <inf> main:   Panel Type: GDEP015OC1
[00:00:00.001,000] <inf> main:   Width: 200 pixels
[00:00:00.001,000] <inf> main:   Height: 200 pixels
[00:00:00.002,000] <inf> main: Test pattern drawn to framebuffer
[00:00:00.002,000] <inf> main: Example complete!
```

## Troubleshooting

- **SPI not working**: Verify pin assignments in device tree overlay
- **Display not responding**: Check power supply (3.3V required) and wiring
- **Build errors**: Ensure Zephyr SDK 4.2.1 or later is installed
