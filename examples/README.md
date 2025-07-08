# GxEPD2 Examples - PlatformIO Project

This is a multi-environment PlatformIO project containing all the GxEPD2 examples converted from Arduino IDE format.

## Project Structure

```
examples/
├── platformio.ini          # Main PlatformIO configuration
├── lib/
│   └── GxEPD2/             # Local GxEPD2 library reference
│       ├── library.json    # Library metadata
│       └── src/            # Symbolic link to ../../../src
├── src/                    # All example source code
│   ├── GxEPD2_HelloWorld/
│   ├── GxEPD2_Example/
│   ├── GxEPD2_GFX_Example/
│   └── ... (all other examples)
└── README.md               # This file
```

## Available Examples

| Environment Name | Description | Board |
|-----------------|-------------|-------|
| `hello_world` | Basic Hello World example | ESP32 |
| `basic_example` | Comprehensive GxEPD2 example | ESP32 |
| `gfx_example` | GFX base class usage example | ESP32 |
| `multi_display` | Multiple display handling | ESP32 |
| `not_paged` | Non-paged display example | ESP32 |
| `paged_callback` | Paged display with callbacks | ESP32 |
| `rotary_callback` | Rotary encoder with callbacks | ESP32 |
| `sd_example` | SD card integration | ESP32 |
| `sd_avr_example` | SD card for AVR boards | Arduino Uno |
| `sd_write_bitmap` | Write bitmaps to SD | ESP32 |
| `serial_flash_example` | Serial flash storage | ESP32 |
| `serial_flash_loader` | Serial flash loader | ESP32 |
| `spiffs_example` | SPIFFS filesystem | ESP32 |
| `spiffs_loader` | SPIFFS loader | ESP32 |
| `u8g2_fonts` | U8G2 fonts example | ESP32 |
| `ws_esp32_driver` | Waveshare ESP32 driver | ESP32 |
| `wifi_example` | WiFi connectivity | ESP32 |
| `wifi_certstore_example` | WiFi with certificate store | ESP32 |
| `wifi_certstore_loader` | Certificate store loader | ESP32 |
| `fast_bw_on_color` | Fast B&W on color displays | ESP32 |
| `mixed_test` | Mixed display test | ESP32 |

## Alternative Board Variants

Some examples also have variants for different boards:
- `*_esp8266` - ESP8266 variants (D1 Mini)
- `*_uno` - Arduino Uno variants  
- `*_esp32s3` - ESP32-S3 variants
- `*_stm32` - STM32 variants

## Usage

### Build a specific example
```bash
pio run -e hello_world
```

### Upload to device
```bash
pio run -e hello_world -t upload
```

### Monitor serial output
```bash
pio device monitor -e hello_world
```

### Build all examples
```bash
pio run
```

### List all available environments
```bash
pio project config
```

## Configuration

### Display Selection
Each example includes display selection headers that you need to configure:
- `GxEPD2_display_selection_new_style.h` (recommended)
- `GxEPD2_display_selection.h` (legacy)
- `GxEPD2_display_selection_added.h` (additional displays)

### Wiring
Check `GxEPD2_wiring_examples.h` in each example for wiring suggestions.

### Board Configuration
Edit `platformio.ini` to:
- Change target boards (e.g., `board = esp32dev`)
- Add/modify build flags
- Adjust library dependencies
- Configure different platforms (ESP8266, STM32, etc.)

## Dependencies

Core dependencies are automatically managed:
- **Adafruit GFX Library** - Required for all examples
- **SD** - For SD card examples
- **U8g2** - For U8G2 fonts example

## Custom Boards

To add support for other boards, create a new environment in `platformio.ini`:

```ini
[env:my_custom_board]
extends = env:hello_world
platform = my_platform
board = my_board
build_flags = 
    ${env.build_flags}
    -DMY_CUSTOM_FLAG
```

## Troubleshooting

### Build Errors
1. Check that your display is properly selected in the display selection headers
2. Verify wiring matches the selected configuration  
3. Ensure correct board and platform in `platformio.ini`

### Upload Errors
1. Check that the correct upload port is detected
2. Verify board is in programming mode if required
3. Check power supply (3.3V required for e-paper displays)

### Display Issues
1. Verify 3.3V power supply to display
2. Check SPI wiring (MOSI, MISO, SCK, CS, DC, RST, BUSY)
3. Ensure proper reset timing in code
4. Check display type selection matches your hardware

## Contributing

To add new examples:
1. Create new directory under `src/`
2. Add corresponding environment in `platformio.ini`
3. Update this README

## License

This project follows the same license as the parent GxEPD2 library (GPL-3.0).
