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

#ifndef _GxEPD2_boards_added_H_
#define _GxEPD2_boards_added_H_

// Arduino DUE

// mapping suggestion for Arduino DUE
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 77, CLK -> 76, DIN -> 75
// SPI pins are on 6 pin 2x3 SPI header

#if defined(ARDUINO_ARCH_SAM)
#define MAX_DISPLAY_BUFFER_SIZE 32768ul // e.g., up to 96k
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, MAX_HEIGHT(GxEPD2_154_T8)> display(GxEPD2_154_T8(//*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, MAX_HEIGHT(GxEPD2_154_M09)> display(GxEPD2_154_M09(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, MAX_HEIGHT(GxEPD2_154_M10)> display(GxEPD2_154_M10(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, MAX_HEIGHT(GxEPD2_213_B72)> display(GxEPD2_213_B72(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, MAX_HEIGHT(GxEPD2_213_B73)> display(GxEPD2_213_B73(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_B74, MAX_HEIGHT(GxEPD2_213_B74)> display(GxEPD2_213_B74(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM0213B74
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_213_M21, MAX_HEIGHT(GxEPD2_213_M21)> display(GxEPD2_213_M21(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213M21
//GxEPD2_BW<GxEPD2_213_T5D, MAX_HEIGHT(GxEPD2_213_T5D)> display(GxEPD2_213_T5D(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213T5D
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> display(GxEPD2_290_T5(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029T5
//GxEPD2_BW<GxEPD2_290_T5D, MAX_HEIGHT(GxEPD2_290_T5D)> display(GxEPD2_290_T5D(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029T5D
//GxEPD2_BW<GxEPD2_290_T94, MAX_HEIGHT(GxEPD2_290_T94)> display(GxEPD2_290_T94(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM029T94
//GxEPD2_BW<GxEPD2_290_T94_V2, MAX_HEIGHT(GxEPD2_290_T94_V2)> display(GxEPD2_290_T94_V2(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM029T94, Waveshare 2.9" V2 variant
//GxEPD2_BW<GxEPD2_290_M06, MAX_HEIGHT(GxEPD2_290_M06)> display(GxEPD2_290_M06(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029M06
//GxEPD2_BW<GxEPD2_260, MAX_HEIGHT(GxEPD2_260)> display(GxEPD2_260(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_371, MAX_HEIGHT(GxEPD2_371)> display(GxEPD2_371(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_420_M01, MAX_HEIGHT(GxEPD2_420_M01)> display(GxEPD2_420_M01(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW042M01
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_583_T8, MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075T7 800x480
//GxEPD2_BW < GxEPD2_1160_T91, MAX_HEIGHT(GxEPD2_1160_T91)> display(GxEPD2_1160_T91(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH116T91 960x640
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_213_Z19c, MAX_HEIGHT_3C(GxEPD2_213_Z19c)> display(GxEPD2_213_Z19c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213Z19
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_290_Z13c, MAX_HEIGHT_3C(GxEPD2_290_Z13c)> display(GxEPD2_290_Z13c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH029Z13
//GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT_3C(GxEPD2_290_C90c)> display(GxEPD2_290_C90c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM029C90
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_3C<GxEPD2_750c_Z08, MAX_HEIGHT_3C(GxEPD2_750c_Z08)> display(GxEPD2_750c_Z08(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, MAX_HEIGHT_3C(GxEPD2_750c_Z90)> display(GxEPD2_750c_Z90(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH075Z90 880x528
// 7-color e-paper
#define MAX_HEIGHT_7C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2)) // 2 pixel per byte
//GxEPD2_7C<GxEPD2_565c, MAX_HEIGHT_7C(GxEPD2_565c)> display(GxEPD2_565c(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // Waveshare 5.65" 7-color
// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
// HRDY -> 7, RST -> 9, CS -> SS(77), SCK -> SCK(76), MOSI -> MOSI(75), MISO -> MISO(74), GND -> GND, 5V -> 5V
// note: 5V supply needs to be exact and strong; 5V over diode from USB (e.g. Wemos D1 mini) doesn't work!
//GxEPD2_BW<GxEPD2_it60, MAX_HEIGHT(GxEPD2_it60)> display(GxEPD2_it60(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_it60_1448x1072, MAX_HEIGHT(GxEPD2_it60_1448x1072)> display(GxEPD2_it60_1448x1072(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
//GxEPD2_BW<GxEPD2_it78_1872x1404, MAX_HEIGHT(GxEPD2_it78_1872x1404)> display(GxEPD2_it78_1872x1404(/*CS=77*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
#endif

// Arduino MKR1000 or MKRZERO

// mapping suggestion for Arduino MKR1000 or MKRZERO
// note: can't use SS on MKR1000: is defined as 24, should be 4
// BUSY -> 5, RST -> 6, DC -> 7, CS-> 4, CLK -> 9, DIN -> 8

#if defined(ARDUINO_ARCH_SAMD)
#define MAX_DISPLAY_BUFFER_SIZE 15000ul // ~15k is a good compromise
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, MAX_HEIGHT(GxEPD2_213_B72)> display(GxEPD2_213_B72(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, MAX_HEIGHT(GxEPD2_213_B73)> display(GxEPD2_213_B73(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_B74, MAX_HEIGHT(GxEPD2_213_B74)> display(GxEPD2_213_B74(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEM0213B74
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_213_M21, MAX_HEIGHT(GxEPD2_213_M21)> display(GxEPD2_213_M21(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0213M21
//GxEPD2_BW<GxEPD2_213_T5D, MAX_HEIGHT(GxEPD2_213_T5D)> display(GxEPD2_213_T5D(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0213T5D
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> display(GxEPD2_290_T5(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW029T5
//GxEPD2_BW<GxEPD2_290_T5D, MAX_HEIGHT(GxEPD2_290_T5D)> display(GxEPD2_290_T5D(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW029T5D
//GxEPD2_BW<GxEPD2_290_T94, MAX_HEIGHT(GxEPD2_290_T94)> display(GxEPD2_290_T94(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEM029T94
//GxEPD2_BW<GxEPD2_290_T94_V2, MAX_HEIGHT(GxEPD2_290_T94_V2)> display(GxEPD2_290_T94_V2(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEM029T94, Waveshare 2.9" V2 variant
//GxEPD2_BW<GxEPD2_290_M06, MAX_HEIGHT(GxEPD2_290_M06)> display(GxEPD2_290_M06(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW029M06
//GxEPD2_BW<GxEPD2_260, MAX_HEIGHT(GxEPD2_260)> display(GxEPD2_260(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_371, MAX_HEIGHT(GxEPD2_371)> display(GxEPD2_371(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_420_M01, MAX_HEIGHT(GxEPD2_420_M01)> display(GxEPD2_420_M01(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW042M01
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_583_T8, MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW075T7 800x480
//GxEPD2_BW < GxEPD2_1160_T91, MAX_HEIGHT(GxEPD2_1160_T91)> display(GxEPD2_1160_T91(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEH116T91 960x640
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_3C<GxEPD2_213_Z19c, MAX_HEIGHT_3C(GxEPD2_213_Z19c)> display(GxEPD2_213_Z19c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW0213Z19
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_3C<GxEPD2_290_Z13c, MAX_HEIGHT_3C(GxEPD2_290_Z13c)> display(GxEPD2_290_Z13c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEH029Z13
//GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT_3C(GxEPD2_290_C90c)> display(GxEPD2_290_C90c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEM029C90
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_3C<GxEPD2_750c_Z08, MAX_HEIGHT_3C(GxEPD2_750c_Z08)> display(GxEPD2_750c_Z08(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, MAX_HEIGHT_3C(GxEPD2_750c_Z90)> display(GxEPD2_750c_Z90(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // GDEH075Z90 880x528
// 7-color e-paper
#define MAX_HEIGHT_7C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2)) // 2 pixel per byte
//GxEPD2_7C<GxEPD2_565c, MAX_HEIGHT_7C(GxEPD2_565c)> display(GxEPD2_565c(/*CS=4*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5)); // Waveshare 5.65" 7-color
// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
// HRDY -> 5, RST -> 6, CS -> 4, SCK -> SCK(9), MOSI -> MOSI(8), MISO -> MISO(10), GND -> GND, 5V -> 5V
// note: 5V supply needs to be exact and strong; 5V over diode from USB (e.g. Wemos D1 mini) doesn't work!
//GxEPD2_BW<GxEPD2_it60, MAX_HEIGHT(GxEPD2_it60)> display(GxEPD2_it60(/*CS=77*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_it60_1448x1072, MAX_HEIGHT(GxEPD2_it60_1448x1072)> display(GxEPD2_it60_1448x1072(/*CS=77*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
//GxEPD2_BW<GxEPD2_it78_1872x1404, MAX_HEIGHT(GxEPD2_it78_1872x1404)> display(GxEPD2_it78_1872x1404(/*CS=77*/ 4, /*DC=*/ 7, /*RST=*/ 6, /*BUSY=*/ 5));
#endif

// mapping suggestion for ESP32, e.g. LOLIN32 D32 PRO
// BUSY -> 15, RST -> 2, DC -> 0, CS -> 5, CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V
// note: use explicit value for CS, as SS is re-defined to TF_CS(4) in pins_arduino.h for Board: "LOLIN D32 PRO"

#if defined(ESP32) && defined(ARDUINO_LOLIN_D32_PRO)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM0213B74
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21::HEIGHT> display(GxEPD2_213_M21(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0213M21
//GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D::HEIGHT> display(GxEPD2_213_T5D(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0213T5D
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW029T5
//GxEPD2_BW<GxEPD2_290_T5D, GxEPD2_290_T5D::HEIGHT> display(GxEPD2_290_T5D(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW029T5D
//GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM029T94
//GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM029T94, Waveshare 2.9" V2 variant
//GxEPD2_BW<GxEPD2_290_M06, GxEPD2_290_M06::HEIGHT> display(GxEPD2_290_M06(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW029M06
//GxEPD2_BW<GxEPD2_260, GxEPD2_260::HEIGHT> display(GxEPD2_260(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_371, GxEPD2_371::HEIGHT> display(GxEPD2_371(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_420_M01, GxEPD2_420_M01::HEIGHT> display(GxEPD2_420_M01(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW042M01
//GxEPD2_BW<GxEPD2_583, GxEPD2_583::HEIGHT> display(GxEPD2_583(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT> display(GxEPD2_583_T8(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW075T7 800x480
//GxEPD2_BW < GxEPD2_1160_T91, GxEPD2_1160_T91::HEIGHT / 2> display(GxEPD2_1160_T91(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEH116T91 960x640
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_3C<GxEPD2_213_Z19c, GxEPD2_213_Z19c::HEIGHT> display(GxEPD2_213_Z19c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW0213Z19
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_3C<GxEPD2_290_Z13c, GxEPD2_290_Z13c::HEIGHT> display(GxEPD2_290_Z13c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEH029Z13
//GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM029C90
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_3C<GxEPD2_583c, GxEPD2_583c::HEIGHT> display(GxEPD2_583c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(GxEPD2_750c_Z08(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, GxEPD2_750c_Z90::HEIGHT> display(GxEPD2_750c_Z90(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEH075Z90 880x528
// 7-color e-paper
//GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // Waveshare 5.65" 7-color
// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
// HRDY -> 4, RST -> 16, CS -> 5, SCK -> SCK(18), MOSI -> MOSI(23), MISO -> MISO(19), GND -> GND, 5V -> 5V
// note: 5V supply needs to be exact and strong; 5V over diode from USB (e.g. Wemos D1 mini) doesn't work!
//GxEPD2_BW<GxEPD2_it60, GxEPD2_it60::HEIGHT> display(GxEPD2_it60(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW < GxEPD2_it60_1448x1072, GxEPD2_it60_1448x1072::HEIGHT / 4 > display(GxEPD2_it60_1448x1072(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
//GxEPD2_BW < GxEPD2_it78_1872x1404, GxEPD2_it78_1872x1404::HEIGHT / 8 > display(GxEPD2_it78_1872x1404(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15));
#endif

// mapping suggestion for ESP32, e.g. TTGO T8 ESP32-WROVER
// BUSY -> 4, RST -> 0, DC -> 2, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V
// for use with Board: "ESP32 Dev Module":

#if defined(ESP32) && defined(ARDUINO_ESP32_DEV)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEM0213B74
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21::HEIGHT> display(GxEPD2_213_M21(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0213M21
//GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D::HEIGHT> display(GxEPD2_213_T5D(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0213T5D
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW029T5
//GxEPD2_BW<GxEPD2_290_T5D, GxEPD2_290_T5D::HEIGHT> display(GxEPD2_290_T5D(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW029T5D
//GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEM029T94
//GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEM029T94, Waveshare 2.9" V2 variant
//GxEPD2_BW<GxEPD2_290_M06, GxEPD2_290_M06::HEIGHT> display(GxEPD2_290_M06(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW029M06
//GxEPD2_BW<GxEPD2_260, GxEPD2_260::HEIGHT> display(GxEPD2_260(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_371, GxEPD2_371::HEIGHT> display(GxEPD2_371(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_420_M01, GxEPD2_420_M01::HEIGHT> display(GxEPD2_420_M01(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW042M01
//GxEPD2_BW<GxEPD2_583, GxEPD2_583::HEIGHT> display(GxEPD2_583(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT> display(GxEPD2_583_T8(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW075T7 800x480
//GxEPD2_BW < GxEPD2_1160_T91, GxEPD2_1160_T91::HEIGHT / 2> display(GxEPD2_1160_T91(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEH116T91 960x640
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_213_Z19c, GxEPD2_213_Z19c::HEIGHT> display(GxEPD2_213_Z19c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW0213Z19
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_290_Z13c, GxEPD2_290_Z13c::HEIGHT> display(GxEPD2_290_Z13c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEH029Z13
//GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEM029C90
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_583c, GxEPD2_583c::HEIGHT> display(GxEPD2_583c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(GxEPD2_750c_Z08(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, GxEPD2_750c_Z90::HEIGHT> display(GxEPD2_750c_Z90(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // GDEH075Z90 880x528
// 7-color e-paper
//GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 2 > display(GxEPD2_565c(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // Waveshare 5.65" 7-color
// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
// HRDY -> 4, RST -> 16, CS -> SS(5), SCK -> SCK(18), MOSI -> MOSI(23), MISO -> MISO(19), GND -> GND, 5V -> 5V
// note: 5V supply needs to be exact and strong; 5V over diode from USB (e.g. Wemos D1 mini) doesn't work!
//GxEPD2_BW<GxEPD2_it60, GxEPD2_it60::HEIGHT> display(GxEPD2_it60(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW < GxEPD2_it60_1448x1072, GxEPD2_it60_1448x1072::HEIGHT / 4 > display(GxEPD2_it60_1448x1072(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
//GxEPD2_BW < GxEPD2_it78_1872x1404, GxEPD2_it78_1872x1404::HEIGHT / 8 > display(GxEPD2_it78_1872x1404(/*CS=5*/ 5, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4));
#endif

// mapping of Waveshare e-Paper ESP8266 Driver Board, old version
// BUSY -> GPIO16, RST -> GPIO5, DC -> GPIO4, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V
// NOTE for ESP8266: using SS (GPIO15) for CS may cause boot mode problems, add a 3.3k pull-down in case
//      the e-Paper ESP8266 Driver Board should have no boot mode issue, as it doesn't use level converters

#if defined (ESP8266)
// select one , can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT> display(GxEPD2_154_T8(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT> display(GxEPD2_154_M09(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, GxEPD2_154_M10::HEIGHT> display(GxEPD2_154_M10(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEM0213B74
//GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> display(GxEPD2_213_flex(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21::HEIGHT> display(GxEPD2_213_M21(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0213M21
//GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D::HEIGHT> display(GxEPD2_213_T5D(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0213T5D
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT> display(GxEPD2_290_T5(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW029T5
//GxEPD2_BW<GxEPD2_290_T5D, GxEPD2_290_T5D::HEIGHT> display(GxEPD2_290_T5D(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW029T5D
//GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEM029T94
//GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEM029T94, Waveshare 2.9" V2 variant
//GxEPD2_BW<GxEPD2_290_M06, GxEPD2_290_M06::HEIGHT> display(GxEPD2_290_M06(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW029M06
//GxEPD2_BW<GxEPD2_260, GxEPD2_260::HEIGHT> display(GxEPD2_260(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_371, GxEPD2_371::HEIGHT> display(GxEPD2_371(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_420_M01, GxEPD2_420_M01::HEIGHT> display(GxEPD2_420_M01(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW042M01
// can use only half buffer size
//GxEPD2_BW < GxEPD2_583, GxEPD2_583::HEIGHT / 2 > display(GxEPD2_583(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW < GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT / 2 > display(GxEPD2_583_T8(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW < GxEPD2_750, GxEPD2_750::HEIGHT / 2 > display(GxEPD2_750(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW < GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT / 2 > display(GxEPD2_750_T7(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW075T7 800x480
//GxEPD2_BW < GxEPD2_1160_T91, GxEPD2_1160_T91::HEIGHT / 2 > display(GxEPD2_1160_T91(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH116T91 960x640
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, GxEPD2_154_Z90c::HEIGHT> display(GxEPD2_154_Z90c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_213_Z19c, GxEPD2_213_Z19c::HEIGHT> display(GxEPD2_213_Z19c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW0213Z19
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_290_Z13c, GxEPD2_290_Z13c::HEIGHT> display(GxEPD2_290_Z13c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH029Z13
//GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEM029C90
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
// can use only half buffer size
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT / 2> display(GxEPD2_420c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
// can use only quarter buffer size
//GxEPD2_3C<GxEPD2_583c, GxEPD2_583c::HEIGHT / 4> display(GxEPD2_583c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT / 4> display(GxEPD2_750c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 4> display(GxEPD2_750c_Z08(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, GxEPD2_750c_Z90::HEIGHT / 4> display(GxEPD2_750c_Z90(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH075Z90 880x528
// 7-color e-paper
//GxEPD2_7C < GxEPD2_565c, GxEPD2_565c::HEIGHT / 4 > display(GxEPD2_565c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // Waveshare 5.65" 7-color
#endif

#if defined(ARDUINO_ARCH_RP2040)
#define MAX_DISPLAY_BUFFER_SIZE 131072ul // e.g. half of available ram
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
//GxEPD2_BW<GxEPD2_154, MAX_HEIGHT(GxEPD2_154)> display(GxEPD2_154(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, MAX_HEIGHT(GxEPD2_154_D67)> display(GxEPD2_154_D67(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_154_T8, MAX_HEIGHT(GxEPD2_154_T8)> display(GxEPD2_154_T8(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154T8 152x152
//GxEPD2_BW<GxEPD2_154_M09, MAX_HEIGHT(GxEPD2_154_M09)> display(GxEPD2_154_M09(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M09 200x200
//GxEPD2_BW<GxEPD2_154_M10, MAX_HEIGHT(GxEPD2_154_M10)> display(GxEPD2_154_M10(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154M10 152x152
//GxEPD2_BW<GxEPD2_213, MAX_HEIGHT(GxEPD2_213)> display(GxEPD2_213(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDE0213B1, phased out
//GxEPD2_BW<GxEPD2_213_B72, MAX_HEIGHT(GxEPD2_213_B72)> display(GxEPD2_213_B72(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0213B72
//GxEPD2_BW<GxEPD2_213_B73, MAX_HEIGHT(GxEPD2_213_B73)> display(GxEPD2_213_B73(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0213B73
//GxEPD2_BW<GxEPD2_213_B74, MAX_HEIGHT(GxEPD2_213_B74)> display(GxEPD2_213_B74(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM0213B74
//GxEPD2_BW<GxEPD2_213_flex, MAX_HEIGHT(GxEPD2_213_flex)> display(GxEPD2_213_flex(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213I5F
//GxEPD2_BW<GxEPD2_213_M21, MAX_HEIGHT(GxEPD2_213_M21)> display(GxEPD2_213_M21(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213M21
//GxEPD2_BW<GxEPD2_213_T5D, MAX_HEIGHT(GxEPD2_213_T5D)> display(GxEPD2_213_T5D(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213T5D
//GxEPD2_BW<GxEPD2_290, MAX_HEIGHT(GxEPD2_290)> display(GxEPD2_290(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH029A1
//GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> display(GxEPD2_290_T5(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029T5
//GxEPD2_BW<GxEPD2_290_T5D, MAX_HEIGHT(GxEPD2_290_T5D)> display(GxEPD2_290_T5D(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029T5D
//GxEPD2_BW<GxEPD2_290_T94, MAX_HEIGHT(GxEPD2_290_T94)> display(GxEPD2_290_T94(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM029T94
//GxEPD2_BW<GxEPD2_290_T94_V2, MAX_HEIGHT(GxEPD2_290_T94_V2)> display(GxEPD2_290_T94_V2(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM029T94, Waveshare 2.9" V2 variant
//GxEPD2_BW<GxEPD2_290_M06, MAX_HEIGHT(GxEPD2_290_M06)> display(GxEPD2_290_M06(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029M06
//GxEPD2_BW<GxEPD2_260, MAX_HEIGHT(GxEPD2_260)> display(GxEPD2_260(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW026T0
//GxEPD2_BW<GxEPD2_260_M01, MAX_HEIGHT(GxEPD2_260_M01)> display(GxEPD2_260_M01(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW026M01
//GxEPD2_BW<GxEPD2_270, MAX_HEIGHT(GxEPD2_270)> display(GxEPD2_270(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW027W3
//GxEPD2_BW<GxEPD2_371, MAX_HEIGHT(GxEPD2_371)> display(GxEPD2_371(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0371W7
//GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW042T2
//GxEPD2_BW<GxEPD2_420_M01, MAX_HEIGHT(GxEPD2_420_M01)> display(GxEPD2_420_M01(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW042M01
//GxEPD2_BW<GxEPD2_583, MAX_HEIGHT(GxEPD2_583)> display(GxEPD2_583(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0583T7
//GxEPD2_BW <GxEPD2_583_T8,  MAX_HEIGHT(GxEPD2_583_T8)> display(GxEPD2_583_T8(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0583T8  648x480
//GxEPD2_BW<GxEPD2_750, MAX_HEIGHT(GxEPD2_750)> display(GxEPD2_750(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075T8   640x384
//GxEPD2_BW<GxEPD2_750_T7, MAX_HEIGHT(GxEPD2_750_T7)> display(GxEPD2_750_T7(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075T7 800x480
//GxEPD2_BW < GxEPD2_1160_T91, MAX_HEIGHT(GxEPD2_1160_T91)> display(GxEPD2_1160_T91(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH116T91 960x640
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
//GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(GxEPD2_154c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0154Z04 no longer available
//GxEPD2_3C<GxEPD2_154_Z90c, MAX_HEIGHT_3C(GxEPD2_154_Z90c)> display(GxEPD2_154_Z90c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH0154Z90
//GxEPD2_3C<GxEPD2_213c, MAX_HEIGHT_3C(GxEPD2_213c)> display(GxEPD2_213c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213Z16
//GxEPD2_3C<GxEPD2_213_Z19c, MAX_HEIGHT_3C(GxEPD2_213_Z19c)> display(GxEPD2_213_Z19c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0213Z19
//GxEPD2_3C<GxEPD2_290c, MAX_HEIGHT_3C(GxEPD2_290c)> display(GxEPD2_290c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW029Z10
//GxEPD2_3C<GxEPD2_290_Z13c, MAX_HEIGHT_3C(GxEPD2_290_Z13c)> display(GxEPD2_290_Z13c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH029Z13
//GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT_3C(GxEPD2_290_C90c)> display(GxEPD2_290_C90c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEM029C90
//GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW027C44
//GxEPD2_3C<GxEPD2_420c, MAX_HEIGHT_3C(GxEPD2_420c)> display(GxEPD2_420c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW042Z15
//GxEPD2_3C<GxEPD2_583c, MAX_HEIGHT_3C(GxEPD2_583c)> display(GxEPD2_583c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW0583Z21
//GxEPD2_3C<GxEPD2_750c, MAX_HEIGHT_3C(GxEPD2_750c)> display(GxEPD2_750c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075Z09
//GxEPD2_3C<GxEPD2_750c_Z08, MAX_HEIGHT_3C(GxEPD2_750c_Z08)> display(GxEPD2_750c_Z08(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEW075Z08 800x480
//GxEPD2_3C<GxEPD2_750c_Z90, MAX_HEIGHT_3C(GxEPD2_750c_Z90)> display(GxEPD2_750c_Z90(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // GDEH075Z90 880x528
// 7-color e-paper
#define MAX_HEIGHT_7C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2)) // 2 pixel per byte
//GxEPD2_7C<GxEPD2_565c, MAX_HEIGHT_7C(GxEPD2_565c)> display(GxEPD2_565c(/*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7)); // Waveshare 5.65" 7-color
#endif

#endif
