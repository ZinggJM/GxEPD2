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

// non-AVR board can also be used with GxEPD2 base display classes, e.g. for SD bitmap drawing

#ifndef _GxEPD2_SD_AVR_boards_added_H_
#define _GxEPD2_SD_AVR_boards_added_H_

#if defined (ESP8266)
#define SD_CS SS  // e.g. for RobotDyn Wemos D1 mini SD board
#define EPD_CS D1 // alternative I use with RobotDyn Wemos D1 mini SD board
// select one and adapt to your mapping
//GxEPD2_154 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4); // GDEP015OC1 no longer available
//GxEPD2_154_D67 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4); // GDEH0154D67
//GxEPD2_213 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_290 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_270 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_420 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_583 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_750 display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
// 3-color e-papers
// !!! can't be used for this //GxEPD2_154c display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_213c display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_290c display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_270c display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_420c display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_583c display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
//GxEPD2_750c display(/*CS=*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4);
#endif

#if defined(ESP32)
#define SD_CS 2  // adapt to your wiring
#define EPD_CS SS // adapt to your wiring
// select one and adapt to your mapping
//GxEPD2_154 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4); // GDEP015OC1 no longer available
//GxEPD2_154_D67 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4); // GDEH0154D67
//GxEPD2_213 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_290 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_270 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_420 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_583 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_750 display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
// 3-color e-papers
// !!! can't be used for this //GxEPD2_154c display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_213c display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_290c display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_270c display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_420c display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_583c display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
//GxEPD2_750c display(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4);
#endif

#if defined(_BOARD_GENERIC_STM32F103C_H_)
#define SD_CS 0  // adapt to your wiring
#define EPD_CS SS // adapt to your wiring
// select one and adapt to your mapping
//GxEPD2_154 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1); // GDEP015OC1 no longer available
//GxEPD2_154_D67 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1); // GDEH0154D67
//GxEPD2_213 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_290 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_270 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_420 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_583 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_750 display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
// 3-color e-papers
// !!! can't be used for this //GxEPD2_154c display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_213c display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_290c display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_270c display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_420c display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_583c display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
//GxEPD2_750c display(/*CS=4*/ EPD_CS, /*DC=*/ 3, /*RST=*/ 2, /*BUSY=*/ 1);
#endif

// Arduino DUE

// mapping suggestion for Arduino DUE
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 77, CLK -> 76, DIN -> 75
// SPI pins are on 6 pin 2x3 SPI header

#if defined(ARDUINO_ARCH_SAM)
#define SD_CS 0  // adapt to your wiring
#define EPD_CS SS // adapt to your wiring
// select one and adapt to your mapping
//GxEPD2_154 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7); // GDEP015OC1 no longer available
//GxEPD2_154_D67 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7); // GDEH0154D67
//GxEPD2_213 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_290 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_270 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_420 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_583 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_750 display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
// 3-color e-papers
// !!! can't be used for this //GxEPD2_154c display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_213c display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_290c display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_270c display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_420c display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_583c display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
//GxEPD2_750c display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7);
#endif

// Arduino MKR1000 or MKRZERO

// mapping suggestion for Arduino MKR1000 or MKRZERO
// note: can't use SS on MKR1000: is defined as 24, should be 4
// BUSY -> 5, RST -> 6, DC -> 7, CS-> 4, CLK -> 9, DIN -> 8

#if defined(ARDUINO_ARCH_SAMD)
#define SD_CS 0  // adapt to your wiring
#define EPD_CS SS // adapt to your wiring
// select one and adapt to your mapping
//GxEPD2_154 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5); // GDEP015OC1 no longer available
//GxEPD2_154_D67 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5); // GDEH0154D67
//GxEPD2_213 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_290 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_270 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_420 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_583 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_750 display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
// 3-color e-papers
// !!! can't be used for this //GxEPD2_154c display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_213c display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_290c display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_270c display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_420c display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_583c display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
//GxEPD2_750c display(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5);
#endif

#endif
