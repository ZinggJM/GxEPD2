#### GxEPD2 : a simple E-Paper display library for Arduino for SPI e-paper displays panels from Dalian Good Display and SPI e-paper display boards from Waveshare
 
#### important note :
#### - these displays are for 3.3V supply and 3.3V data lines
#### - never connect data lines directly to 5V Arduino data pins, you may use e.g. 4k7 series resistor
#### - do not forget to connect GND

## mapping suggestions

#### mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
##### - BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

#### mapping suggestion from Waveshare SPI e-Paper to generic ESP8266
##### - BUSY -> GPIO4, RST -> GPIO2, DC -> GPIO0, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V

#### mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
##### - NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
##### - BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

#### mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
##### - BUSY -> A1, RST -> A2, DC -> A3, CS-> A4, CLK -> A5, DIN -> A7

#### mapping suggestion for AVR, UNO, NANO etc.
##### - BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 13, DIN -> 11

## connection scheme for DESTM32-S2 connection board for e-paper panels:

```
   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display



   DESTM32-S2 pinout (top, component side view):

         |-------------------------------------------------

         |  VCC  |o o| VCC 5V  not needed

         |  GND  |o o| GND     GND

         |  3.3  |o o| 3.3     3.3V

         |  nc   |o o| nc

         |  nc   |o o| nc

         |  nc   |o o| nc

   MOSI  |  DIN  |o o| CLK     SCK

   SS    |  CS   |o o| DC      e.g. D3

   D4    |  RST  |o o| BUSY    e.g. D2

         |  nc   |o o| BS      GND

         |-------------------------------------------------

```
