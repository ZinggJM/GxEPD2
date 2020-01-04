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

#define GxEPD_BLACK     0x0000
#define GxEPD_DARKGREY  0x7BEF      /* 128, 128, 128 */
#define GxEPD_LIGHTGREY 0xC618      /* 192, 192, 192 */
#define GxEPD_WHITE     0xFFFF
#define GxEPD_RED       0xF800      /* 255,   0,   0 */

#define GxEPD_YELLOW    GxEPD_RED
#define GxEPD_COLORED   GxEPD_RED

class GxEPD2
{
  public:
    enum Panel
    {
      GDEP015OC1, Waveshare_1_54_bw = GDEP015OC1,
      GDEH0154D67, Waveshare_1_54_bw_D67 = GDEH0154D67,
      GDE0213B1,  Waveshare_2_13_bw = GDE0213B1,
      GDEH0213B72,  Waveshare_2_13_bw_B72 = GDEH0213B72,
      GDEH0213B73,  Waveshare_2_13_bw_B73 = GDEH0213B73,
      GDEW0213I5F, Waveshare_2_13_flex = GDEW0213I5F,
      GDEW026T0,  Waveshare_2_6_bw = GDEW026T0,
      GDEH029A1,  Waveshare_2_9_bw = GDEH029A1,
      GDEW029T5,  Waveshare_2_9_bw_T5 = GDEW029T5,
      GDEW027W3,  Waveshare_2_7_bw = GDEW027W3,
      GDEW0371W7, Waveshare_3_7_bw = GDEW0371W7,
      GDEW042T2,  Waveshare_4_2_bw = GDEW042T2,
      GDEW0583T7, Waveshare_5_83_bw = GDEW0583T7,
      GDEW075T8,  Waveshare_7_5_bw = GDEW075T8,
      GDEW075T7,  Waveshare_7_5_bw_T7 = GDEW075T7,
      ED060SCT, // on Waveshare IT8951 Driver HAT
      // 3-color
      GDEW0154Z04, Waveshare_1_54_bwr = GDEW0154Z04,
      GDEW0213Z16, Waveshare_2_13_bwr = GDEW0213Z16,
      GDEW029Z10,  Waveshare_2_9_bwr = GDEW029Z10,
      GDEW027C44,  Waveshare_2_7_bwr = GDEW027C44,
      GDEW042Z15,  Waveshare_4_2_bwr = GDEW042Z15,
      GDEW0583Z21, Waveshare_5_83_bwr = GDEW0583Z21,
      GDEW075Z09,  Waveshare_7_5_bwr = GDEW075Z09,
      GDEW075Z08,  Waveshare_7_5_bwr_Z08 = GDEW075Z08
    };
};

#endif
