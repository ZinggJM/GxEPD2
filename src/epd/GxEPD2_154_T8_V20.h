// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display, available here: http://www.e-paper-display.com/download_detail/downloadsId=806.html
// Panel: HTEW0154T8_V20 : https://heltec.org/project/154-e-ink/
// Controller : IL0373 : https://resource.heltec.cn/download/e-ink/154/1.54b%26w/HTEW0154T8_V20/IL0373.pdf
// Display: HELTEC Automation 1.54 inch e-paper module : https://es.aliexpress.com/item/4000594878838.html
//
// Author: Jean-Marc Zingg
// Edited: TheRoam by The Roaming Workshop
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_154_T8_V20_H_
#define _GxEPD2_154_T8_V20_H_

#include "../GxEPD2_EPD.h"

class GxEPD2_154_T8_V20 : public GxEPD2_EPD
{
public:
  // attributes
  static const uint16_t WIDTH = 160;
  static const uint16_t HEIGHT = 160;
  static const GxEPD2::Panel panel = GxEPD2::HTEW0154T8_V20;
  static const bool hasColor = false;
  static const bool hasPartialUpdate = true;
  static const bool hasFastPartialUpdate = true;
  static const uint16_t power_on_time = 100;        // ms, e.g. 94000us
  static const uint16_t power_off_time = 150;       // ms, e.g. 140000us
  static const uint16_t full_refresh_time = 2500;   // ms, e.g. 3825000us
  static const uint16_t partial_refresh_time = 350; // ms, e.g. 736000us
  // constructor
  GxEPD2_154_T8_V20(int16_t cs, int16_t dc, int16_t rst, int16_t busy);
  // methods (virtual)
  //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
  void clearScreen(uint8_t value = 0xFF);            // init controller memory and screen (default white)
  void writeScreenBuffer(uint8_t value = 0xFF);      // init controller memory (default white)
  void writeScreenBufferAgain(uint8_t value = 0xFF); // init previous buffer controller memory (default white)
  // write to controller memory, without screen refresh; x and w should be multiple of 8
  void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void writeImageForFullRefresh(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                      int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void writeImage(const uint8_t *black, const uint8_t *color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void writeImagePart(const uint8_t *black, const uint8_t *color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                      int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  // for differential update: set current and previous buffers equal (for fast partial update to work correctly)
  void writeImageAgain(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void writeImagePartAgain(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                           int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  // write sprite of native data to controller memory, without screen refresh; x and w should be multiple of 8
  void writeNative(const uint8_t *data1, const uint8_t *data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  // write to controller memory, with screen refresh; x and w should be multiple of 8
  void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void drawImage(const uint8_t *black, const uint8_t *color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void drawImagePart(const uint8_t *black, const uint8_t *color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  // write sprite of native data to controller memory, with screen refresh; x and w should be multiple of 8
  void drawNative(const uint8_t *data1, const uint8_t *data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void refresh(bool partial_update_mode = false);           // screen refresh from controller memory to full screen
  void refresh(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, partial screen
  void powerOff();                                          // turns off generation of panel driving voltages, avoids screen fading over time
  void hibernate();                                         // turns powerOff() and sets controller to deep sleep for minimum power use, ONLY if wakeable by RST (rst >= 0)
private:
  void _writeScreenBuffer(uint8_t command, uint8_t value);
  void _writeImage(uint8_t command, const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void _writeImagePart(uint8_t command, const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
  void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void _PowerOn();
  void _PowerOff();
  void _InitDisplay();
  void _Init_Full();
  void _Init_Part();
  void _Update_Full();
  void _Update_Part();

private:
  static const unsigned char lut_partial[];
};

#endif