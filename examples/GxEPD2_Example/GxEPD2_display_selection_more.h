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

#ifndef _GxEPD2_more_boards_added_H_
#define _GxEPD2_more_boards_added_H_
//GxEPD2_BW < GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT / 2 > display(GxEPD2_750_T7(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));

// Wemos D1 R32 Arduino board
// NOTE: pin 14 doesn't work on my board, can't be used for BUSY

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 R32 Arduino board
// BUSY -> 16, RST -> 13, DC -> 12, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

#if defined(ESP32)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=5*/ 15, /*DC=*/ 12, /*RST=*/ 27, /*BUSY=*/ 32)); // HUZZAH32
#define RST_PIN        33
#define DC_PIN         14
#define CS_PIN         15
#define BUSY_PIN       23
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=5*/ CS_PIN, /*DC=*/ DC_PIN, /*RST=*/ 12, /*BUSY=*/ BUSY_PIN)); // HUZZAH32 test2
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16)); // GDEW075T7 800x480
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
// HRDY -> 16, RST -> 13, CS -> SS(5), SCK -> SCK(18), MOSI -> MOSI(23), MISO -> MISO(19), GND -> GND, 5V -> 5V
// note: 5V supply needs to be exact and strong; 5V over diode from USB (e.g. Wemos D1 mini) doesn't work!
//GxEPD2_BW<GxEPD2_it60, GxEPD2_it60::HEIGHT> display(GxEPD2_it60(/*CS=5*/ SS, /*DC=*/ 12, /*RST=*/ 13, /*BUSY=*/ 16));
#endif

// mapping of Waveshare ESP32 Driver Board
// BUSY -> 25, RST -> 26, DC -> 27, CS-> 15, CLK -> 13, DIN -> 14
// BUSY -> 25, RST -> 26, DC -> 27, CS-> 15, CLK -> 18, DIN -> 23 // HW SPI for GxEPD2
// needs connections from pin 13 to 18 and 14 to 23
// does not work because of pin 14 on WROOM-32, like on Wemos D1 R32 Arduino board

#if defined(ESP32)
// ***** for mapping of Waveshare ESP32 Driver Board *****
// use Board "ESP32 Dev Module" to build with Arduino IDE
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEW029T5
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEW075T7 800x480
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
#endif

// Arduino DUE

// mapping suggestion for Arduino DUE, variant with CS on 10
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 76, DIN -> 75
// SPI pins are on 6 pin 2x3 SPI header, except CS

#if defined(ARDUINO_ARCH_SAM)
#define MAX_DISPLAY_BUFFER_SIZE 15000ul // ~15k is a good compromise
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, MAX_HEIGHT(GxEPD2_154_T8)> display(GxEPD2_154_T8(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, MAX_HEIGHT(GxEPD2_154_M09)> display(GxEPD2_154_M09(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, MAX_HEIGHT(GxEPD2_154_M10)> display(GxEPD2_154_M10(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_583_T8, MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075T7 800x480
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=10*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
#endif

// paged drawing test on ESP8266

#if defined (ESP8266)
#define MAX_DISPLAY_BUFFER_SIZE 800 // 
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, MAX_HEIGHT(GxEPD2_154_T8)> display(GxEPD2_154_T8(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, MAX_HEIGHT(GxEPD2_154_M09)> display(GxEPD2_154_M09(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, MAX_HEIGHT(GxEPD2_154_M10)> display(GxEPD2_154_M10(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> display(GxEPD2_290_T5(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW029T5
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_583_T8, MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW075T7 800x480
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
#endif

#if defined(ARDUINO_ARCH_STM32) && (defined(ARDUINO_NUCLEO_F103RB))
// package STM32 Cores by STMicroelecronics version 1.5.0
// Board Manager section: "STM32 Boards (selected from submenu)"
// Board: "Nucleo-64"
// Board part number: "Nucleo_F103RB"
// Note: set SPI jumper switches to "on" for pin 11, 12, 13 use (no ICSP connector on Nucleo)
#define MAX_DISPLAY_BUFFER_SIZE 15000ul // ~15k is a good compromise
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, MAX_HEIGHT(GxEPD2_154_T8)> display(GxEPD2_154_T8(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, MAX_HEIGHT(GxEPD2_154_M09)> display(GxEPD2_154_M09(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, MAX_HEIGHT(GxEPD2_154_M10)> display(GxEPD2_154_M10(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, MAX_HEIGHT(GxEPD2_213_B72)> display(GxEPD2_213_B72(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> display(GxEPD2_290_T5(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEW029T5
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_583_T8, MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEW075T7 800x480
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=10*/ SS, /*DC=*/ 9, /*RST=*/ 8, /*BUSY=*/ 7));
#endif

#endif
