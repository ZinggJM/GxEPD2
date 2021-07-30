// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_H_
#define _GxEPD2_H_

#include <Arduino.h>
#include <SPI.h>

// color definitions for GxEPD, GxEPD2 and GxEPD_HD, values correspond to RGB565 values for TFTs
#define GxEPD_BLACK     0x0000
#define GxEPD_WHITE     0xFFFF
// some controllers for b/w EPDs support grey levels
#define GxEPD_DARKGREY  0x7BEF // 128, 128, 128
#define GxEPD_LIGHTGREY 0xC618 // 192, 192, 192
// values for 3-color or 7-color EPDs
#define GxEPD_RED       0xF800 // 255,   0,   0
#define GxEPD_YELLOW    0xFFE0 // 255, 255,   0 !!no longer same as GxEPD_RED!!
#define GxEPD_COLORED   GxEPD_RED
// values for 7-color EPDs only
#define GxEPD_BLUE      0x001F //   0,   0, 255
#define GxEPD_GREEN     0x07E0 //   0, 255,   0
#define GxEPD_ORANGE    0xFD20 // 255, 165,   0

class GxEPD2
{
  public:
    enum Panel
    {
      GDEP015OC1, Waveshare_1_54_bw = GDEP015OC1,
      GDEH0154D67, Waveshare_1_54_bw_D67 = GDEH0154D67,
      GDEW0154T8,
      GDEW0154M09,
      GDEW0154M10,
      GDE0213B1,  Waveshare_2_13_bw = GDE0213B1,
      GDEH0213B72,  Waveshare_2_13_bw_B72 = GDEH0213B72,
      GDEH0213B73,  Waveshare_2_13_bw_B73 = GDEH0213B73,
      GDEM0213B74,
      GDEW0213I5F, Waveshare_2_13_flex = GDEW0213I5F,
      GDEW0213M21,
      GDEW0213T5D,
      GDEW026T0,  Waveshare_2_6_bw = GDEW026T0,
      GDEW026M01,
      GDEH029A1,  Waveshare_2_9_bw = GDEH029A1,
      GDEW029T5,  Waveshare_2_9_bw_T5 = GDEW029T5,
      GDEW029T5D,
      GDEW029M06,
      GDEM029T94,
      GDEW027W3,  Waveshare_2_7_bw = GDEW027W3,
      GDEW0371W7, Waveshare_3_7_bw = GDEW0371W7,
      GDEW042T2,  Waveshare_4_2_bw = GDEW042T2,
      GDEW042M01,
      GDEW0583T7, Waveshare_5_83_bw = GDEW0583T7,
      GDEW0583T8,
      GDEW075T8,  Waveshare_7_5_bw = GDEW075T8,
      GDEW075T7,  Waveshare_7_5_bw_T7 = GDEW075T7,
      GDEH116T91,
      GDEW1248T3, Waveshare_12_24_bw = GDEW1248T3,
      ED060SCT, // on Waveshare IT8951 Driver HAT
      ED060KC1, // on Waveshare IT8951 Driver HAT 1448x1072
      ED078KC2, // on Waveshare IT8951 Driver HAT 1872x1404
      // 3-color
      GDEW0154Z04, Waveshare_1_54_bwr = GDEW0154Z04,
      GDEH0154Z90, Waveshare_1_54_bwr_Z90 = GDEH0154Z90,
      GDEW0213Z16, Waveshare_2_13_bwr = GDEW0213Z16,
      GDEW0213Z19,
      GDEW029Z10,  Waveshare_2_9_bwr = GDEW029Z10,
      GDEH029Z13,
      GDEM029C90,
      GDEW027C44,  Waveshare_2_7_bwr = GDEW027C44,
      GDEW042Z15,  Waveshare_4_2_bwr = GDEW042Z15,
      GDEW0583Z21, Waveshare_5_83_bwr = GDEW0583Z21,
      ACeP565,     Waveshare_5_65_7c = ACeP565,
      GDEW075Z09,  Waveshare_7_5_bwr = GDEW075Z09,
      GDEW075Z08,  Waveshare_7_5_bwr_Z08 = GDEW075Z08,
      GDEH075Z90,  Waveshare_7_5_bwr_Z90 = GDEH075Z90
    };
};

#endif
