// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// GxEPD2_U8G2_Fonts_Example : show use of U8G2_FOR_ADAFRUIT_GFX with GxEPD2
//
// Library: https://github.com/olikraus/U8g2_for_Adafruit_GFX
// see: https://github.com/olikraus/U8g2_for_Adafruit_GFX/blob/master/README.md
//
// NOTE: you need to SAVE the modified example to a saveable location for UTF-8 characters to work
//       e.g. for Umlauts

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Display ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V
// NOTE: connect 3.3k pull-down from D8 to GND if your board or shield has level converters
// NOTE for ESP8266: using SS (GPIO15) for CS may cause boot mode problems, use different pin in case, or 4k7 pull-down

// mapping suggestion from Waveshare SPI e-Paper to generic ESP8266
// BUSY -> GPIO4, RST -> GPIO2, DC -> GPIO0, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V
// NOTE: connect 3.3k pull-down from GPIO15 to GND if your board or shield has level converters
// NOTE for ESP8266: using SS (GPIO15) for CS may cause boot mode problems, use different pin in case, or 3.3k pull-down

// mapping of Waveshare e-Paper ESP8266 Driver Board, new version (see GxEPD2_boards_added.h for old version)
// BUSY -> GPIO5, RST -> GPIO2, DC -> GPIO4, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V
// NOTE for ESP8266: using SS (GPIO15) for CS may cause boot mode problems, add a 3.3k pull-down in case
//      the e-Paper ESP8266 Driver Board should have no boot mode issue, as it doesn't use level converters

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// mapping of Waveshare ESP32 Driver Board
// BUSY -> 25, RST -> 26, DC -> 27, CS-> 15, CLK -> 13, DIN -> 14
// NOTE: this board uses "unusual" SPI pins and requires re-mapping of HW SPI to these pins in SPIClass
//       see example GxEPD2_WS_ESP32_Driver.ino, it shows how this can be done easily

// new mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
// BUSY -> A1, RST -> A2, DC -> A3, CS-> A4, CLK -> A5, DIN -> A7

// mapping suggestion for AVR, UNO, NANO etc.
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 13, DIN -> 11

// mapping of Waveshare Universal e-Paper Raw Panel Driver Shield for Arduino / NUCLEO
// BUSY -> 7, RST -> 8, DC -> 9, CS-> 10, CLK -> 13, DIN -> 11

// mapping suggestion for Arduino MEGA
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 53, CLK -> 52, DIN -> 51

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <U8g2_for_Adafruit_GFX.h>

#if defined (ESP8266)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW029T5
//GxEPD2_BW<GxEPD2_260, GxEPD2_260::HEIGHT> display(GxEPD2_260(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_371, GxEPD2_371::HEIGHT> display(GxEPD2_371(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// can use only half buffer size
//GxEPD2_BW < GxEPD2_583, GxEPD2_583::HEIGHT / 2 > display(GxEPD2_583(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW < GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT / 2 > display(GxEPD2_583_T8(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW < GxEPD2_750, GxEPD2_750::HEIGHT / 2 > display(GxEPD2_750(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW < GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT / 2 > display(GxEPD2_750_T7(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW075T7 800x480
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// can use only half buffer size
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT / 2> display(GxEPD2_420c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// can use only quarter buffer size
//GxEPD2_3C < GxEPD2_583c, GxEPD2_583c::HEIGHT / 4 > display(GxEPD2_583c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT / 4> display(GxEPD2_750c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C < GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 4 > display(GxEPD2_750c_Z08(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW075Z08 800x480
//GxEPD2_3C < GxEPD2_750c_Z90, GxEPD2_750c_Z90::HEIGHT / 4 > display(GxEPD2_750c_Z90(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH075Z90 880x528
// 7-color e-paper
//GxEPD2_3C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 4 > display(GxEPD2_565c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // Waveshare 5.65" 7-color (3C graphics)
//GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 4 > display(GxEPD2_565c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // Waveshare 5.65" 7-color
// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
// HRDY -> 4, RST -> 2, CS -> SS(15), SCK -> SCK(14), MOSI -> MOSI(D7(13)), MISO -> MISO(D6(12)), GND -> GND, 5V -> 5V
// note: 5V supply needs to be exact and strong; 5V pin of USB powered Wemos D1 mini doesn't work!
//GxEPD2_BW<GxEPD2_it60, GxEPD2_it60::HEIGHT / 8> display(GxEPD2_it60(/*CS=5*/ SS, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 4));

// ***** for mapping of Waveshare e-Paper ESP8266 Driver Board *****
// select one , can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW029T5
//GxEPD2_BW<GxEPD2_260, GxEPD2_260::HEIGHT> display(GxEPD2_260(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_BW<GxEPD2_371, GxEPD2_371::HEIGHT> display(GxEPD2_371(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
// can use only half buffer size
//GxEPD2_BW < GxEPD2_583, GxEPD2_583::HEIGHT / 2 > display(GxEPD2_583(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_BW < GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT / 2 > display(GxEPD2_583_T8(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_BW < GxEPD2_750, GxEPD2_750::HEIGHT / 2 > display(GxEPD2_750(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_BW < GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT / 2 > display(GxEPD2_750_T7(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW075T7 800x480
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
// can use only half buffer size
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT / 2> display(GxEPD2_420c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
// can use only quarter buffer size
//GxEPD2_3C<GxEPD2_583c, GxEPD2_583c::HEIGHT / 4> display(GxEPD2_583c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT / 4> display(GxEPD2_750c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
//GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 4> display(GxEPD2_750c_Z08(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, GxEPD2_750c_Z90::HEIGHT / 4> display(GxEPD2_750c_Z90(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // GDEH075Z90 880x528
// 7-color e-paper
//GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 4 > display(GxEPD2_565c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5)); // Waveshare 5.65" 7-color
#endif

#if defined(ESP32)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW029T5
//GxEPD2_BW<GxEPD2_260, GxEPD2_260::HEIGHT> display(GxEPD2_260(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_371, GxEPD2_371::HEIGHT> display(GxEPD2_371(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_583, GxEPD2_583::HEIGHT> display(GxEPD2_583(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT> display(GxEPD2_583_T8(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW075T7 800x480
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_583c, GxEPD2_583c::HEIGHT> display(GxEPD2_583c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(GxEPD2_750c_Z08(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, GxEPD2_750c_Z90::HEIGHT / 2> display(GxEPD2_750c_Z90(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH075Z90 880x528
// 7-color e-paper
//GxEPD2_3C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // Waveshare 5.65" 7-color (3C graphics)
//GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // Waveshare 5.65" 7-color

// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
// HRDY -> 4, RST -> 16, CS -> SS(5), SCK -> SCK(18), MOSI -> MOSI(23), MISO -> MISO(19), GND -> GND, 5V -> 5V
// note: 5V supply needs to be exact and strong; 5V over diode from USB (e.g. Wemos D1 mini) doesn't work!
//GxEPD2_BW<GxEPD2_it60, GxEPD2_it60::HEIGHT> display(GxEPD2_it60(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

// Waveshare 12.48 b/w SPI display board and frame or Good Display 12.48 b/w panel GDEW1248T3
// general constructor for use with all parameters, e.g. for Waveshare ESP32 driver board mounted on connection board
//GxEPD2_BW < GxEPD2_1248, GxEPD2_1248::HEIGHT / 4 >
//display(GxEPD2_1248(/*sck=*/ 13, /*miso=*/ 12, /*mosi=*/ 14, /*cs_m1=*/ 23, /*cs_s1=*/ 22, /*cs_m2=*/ 16, /*cs_s2=*/ 19,
//                             /*dc1=*/ 25, /*dc2=*/ 17, /*rst1=*/ 33, /*rst2=*/ 5, /*busy_m1=*/ 32, /*busy_s1=*/ 26, /*busy_m2=*/ 18, /*busy_s2=*/ 4));
#endif

// can't use package "STMF1 Boards (STM32Duino.com)" (Roger Clark) anymore with Adafruit_GFX, use "STM32 Boards (selected from submenu)" (STMicroelectronics)
#if defined(ARDUINO_ARCH_STM32)
#define MAX_DISPLAY_BUFFER_SIZE 15000ul // ~15k is a good compromise
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, MAX_HEIGHT(GxEPD2_154_T8)> display(GxEPD2_154_T8(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, MAX_HEIGHT(GxEPD2_154_M09)> display(GxEPD2_154_M09(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, MAX_HEIGHT(GxEPD2_154_M10)> display(GxEPD2_154_M10(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, MAX_HEIGHT(GxEPD2_213_B72)> display(GxEPD2_213_B72(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, MAX_HEIGHT(GxEPD2_213_B73)> display(GxEPD2_213_B73(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> display(GxEPD2_290_T5(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW029T5
//GxEPD2_BW<GxEPD2_260, MAX_HEIGHT(GxEPD2_260)> display(GxEPD2_260(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW<GxEPD2_371, MAX_HEIGHT(GxEPD2_371)> display(GxEPD2_371(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW <GxEPD2_583_T8,  MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW075T7 800x480
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1));
//GxEPD2_3C<GxEPD2_750c_Z08, MAX_HEIGHT_3C(GxEPD2_750c_Z08)> display(GxEPD2_750c_Z08(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, MAX_HEIGHT_3C(GxEPD2_750c_Z90)> display(GxEPD2_750c_Z90(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // GDEH075Z90 880x528
// 7-color e-paper
#define MAX_HEIGHT_7C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2)) // 2 pixel per byte
//GxEPD2_7C<GxEPD2_565c, MAX_HEIGHT_7C(GxEPD2_565c)> display(GxEPD2_565c(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1)); // Waveshare 5.65" 7-color
#endif

#if defined(__AVR)
#if defined (ARDUINO_AVR_MEGA2560)
#define MAX_DISPLAY_BUFFER_SIZE 5000 // e.g. full height for 200x200
#else
#define MAX_DISPLAY_BUFFER_SIZE 800 // 
#endif
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, MAX_HEIGHT(GxEPD2_154_T8)> display(GxEPD2_154_T8(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, MAX_HEIGHT(GxEPD2_154_M09)> display(GxEPD2_154_M09(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, MAX_HEIGHT(GxEPD2_154_M10)> display(GxEPD2_154_M10(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, MAX_HEIGHT(GxEPD2_213_B72)> display(GxEPD2_213_B72(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, MAX_HEIGHT(GxEPD2_213_B73)> display(GxEPD2_213_B73(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> display(GxEPD2_290_T5(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029T5
//GxEPD2_BW<GxEPD2_260, MAX_HEIGHT(GxEPD2_260)> display(GxEPD2_260(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_371, MAX_HEIGHT(GxEPD2_371)> display(GxEPD2_371(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW <GxEPD2_583_T8,  MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075T7 800x480
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_750c_Z08, MAX_HEIGHT_3C(GxEPD2_750c_Z08)> display(GxEPD2_750c_Z08(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, MAX_HEIGHT_3C(GxEPD2_750c_Z90)> display(GxEPD2_750c_Z90(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH075Z90 880x528
// 7-color e-paper
#define MAX_HEIGHT_7C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2)) // 2 pixel per byte
//GxEPD2_7C<GxEPD2_565c, MAX_HEIGHT_7C(GxEPD2_565c)> display(GxEPD2_565c(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // Waveshare 5.65" 7-color
#endif

#include "GxEPD2_boards_added.h"

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init();
  u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
  helloWorld();
  delay(1000);
  helloArduino();
  delay(1000);
  helloEpaper();
  delay(1000);
  showFont("u8g2_font_helvR14_tf", u8g2_font_helvR14_tf); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  delay(2000);
  showFont("u8g2_font_profont22_mr", u8g2_font_profont22_mr); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  delay(1000);
  Serial.println("setup done");
}

void loop()
{
}

void helloWorld()
{
  //Serial.println("helloWorld");
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  uint16_t x = (display.width() - 160) / 2;
  uint16_t y = display.height() / 2;
  display.firstPage();
  do
  {
    display.fillScreen(bg);
    u8g2Fonts.setCursor(x, y); // start writing at this position
    u8g2Fonts.print("Hello World!");
  }
  while (display.nextPage());
  //Serial.println("helloWorld done");
}

void helloArduino()
{
  //Serial.println("helloArduino");
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  uint16_t x = (display.width() - 160) / 2;
  uint16_t y = display.height() / 4;
  display.setPartialWindow(0, y - 14, display.width(), 20);
  display.firstPage();
  do
  {
    display.fillScreen(bg);
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.println("Hello Arduino!");
  }
  while (display.nextPage());
  //Serial.println("helloArduino done");
}

void helloEpaper()
{
  //Serial.println("helloEpaper");
  uint16_t bg = GxEPD_WHITE;
  uint16_t fg = GxEPD_BLACK;
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  uint16_t x = (display.width() - 160) / 2;
  uint16_t y = display.height() * 3 / 4;
  display.setPartialWindow(0, y - 14, display.width(), 20);
  display.firstPage();
  do
  {
    display.fillScreen(bg);
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.println("Hello E-Paper!");
  }
  while (display.nextPage());
  //Serial.println("helloEpaper done");
}

void showFont(const char name[], const uint8_t *font)
{
  display.setFullWindow();
  display.setRotation(0);
  u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);              // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
  u8g2Fonts.setFont(font); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  display.firstPage();
  do
  {
    drawFont(name);
  }
  while (display.nextPage());
}

void drawFont(const char name[])
{
  //display.setRotation(0);
  display.fillScreen(GxEPD_WHITE);
  u8g2Fonts.setCursor(0, 0);
  u8g2Fonts.println();
  u8g2Fonts.println(name);
  u8g2Fonts.println(" !\"#$%&'()*+,-./");
  u8g2Fonts.println("0123456789:;<=>?");
  u8g2Fonts.println("@ABCDEFGHIJKLMNO");
  u8g2Fonts.println("PQRSTUVWXYZ[\\]^_");
  u8g2Fonts.println("`abcdefghijklmno");
  u8g2Fonts.println("pqrstuvwxyz{|}~ ");
  u8g2Fonts.println("Umlaut ÄÖÜäéöü");
}
