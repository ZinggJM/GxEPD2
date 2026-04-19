// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: https://www.good-display.com/comp/xcompanyFile/downloadNew.do?appId=24&fid=2389&id=1782
// Panel: GDEH0576T81 : https://www.good-display.com/product/702.html
// Controller: SSD2677 : https://v4.cecdn.yun300.cn/100001_1909185148/SSD2677%28Rev1.1%29N.pdf
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_576_GDEH0576T81_H_
#define _GxEPD2_576_GDEH0576T81_H_

#include "../GxEPD2_EPD.h"

class GxEPD2_576_GDEH0576T81 : public GxEPD2_EPD
{
  public:
    // attributes
    static const uint16_t WIDTH = 920;
    static const uint16_t WIDTH_VISIBLE = WIDTH;
    static const uint16_t HEIGHT = 680;
    static const GxEPD2::Panel panel = GxEPD2::GDEH0576T81;
    static const bool hasColor = false;
    static const bool hasPartialUpdate = true;
    static const bool hasFastPartialUpdate = false;
    static const uint16_t power_on_time = 100; // ms, e.g. 81999us
    static const uint16_t power_off_time = 150; // ms, e.g. 140001us
    static const uint16_t full_refresh_time = 1200; // ms, e.g. 1048001us
    static const uint16_t partial_refresh_time = 400; // ms, e.g. 400001us
    // constructor
    GxEPD2_576_GDEH0576T81(int16_t cs, int16_t dc, int16_t rst, int16_t busy);
    // methods (virtual)
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    void clearScreen(uint8_t black_value, uint8_t color_value); // init controller memory and screen
    void writeScreenBuffer(uint8_t value = 0xFF); // init controller memory (default white)
    void writeScreenBuffer(uint8_t black_value, uint8_t color_value); // init controller memory
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, without screen refresh; x and w should be multiple of 8
    // for this controller used to transfer bitmaps of previous and current content for differential refresh
    void writeNative(const uint8_t* previous, const uint8_t* current, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeNativePart(const uint8_t* previous, const uint8_t* current, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                         int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, with screen refresh; x and w should be multiple of 8
    // for this controller used to transfer bitmaps of previous and current content for differential refresh
    void drawNative(const uint8_t* previous, const uint8_t* current, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void refresh(bool partial_update_mode = false); // screen refresh from controller memory to full screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, partial screen
    void powerOff(); // turns off generation of panel driving voltages, avoids screen fading over time
    void hibernate(); // turns powerOff() and sets controller to deep sleep for minimum power use, ONLY if wakeable by RST (rst >= 0)
  private:
    void _refresh();
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial_mode = true);
    void _PowerOn();
    void _PowerOff();
    void _InitDisplay();
    void _InitDisplayDifferential();
    void _use_differential_refresh(bool yes);
    uint8_t _get_lut_temperature();
  private:
    bool _using_differential_refresh;
    uint8_t _lut_temperature; // value for LUT selection
};

#endif
