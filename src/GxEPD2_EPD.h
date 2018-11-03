// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_EPD_H_
#define _GxEPD2_EPD_H_

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
      GDE0213B1,  Waveshare_2_13_bw = GDE0213B1,
      GDEW0213I5F, Waveshare_2_13_flex = GDEW0213I5F,
      GDEH029A1,  Waveshare_2_9_bw = GDEH029A1,
      GDEW027W3,  Waveshare_2_7_bw = GDEW027W3,
      GDEW042T2,  Waveshare_4_2_bw = GDEW042T2,
      GDEW0583T7, Waveshare_5_83_bw = GDEW0583T7,
      GDEW075T8,  Waveshare_7_5_bw = GDEW075T8,
      // 3-color
      GDEW0154Z04, Waveshare_1_54_bwr = GDEW0154Z04,
      GDEW0213Z16, Waveshare_2_13_bwr = GDEW0213Z16,
      GDEW029Z10,  Waveshare_2_9_bwr = GDEW029Z10,
      GDEW027C44,  Waveshare_2_7_bwr = GDEW027C44,
      GDEW042Z15,  Waveshare_4_2_bwr = GDEW042Z15,
      GDEW0583Z21, Waveshare_5_83_bwr = GDEW0583Z21,
      GDEW075Z09,  Waveshare_7_5_bwr = GDEW075Z09
    };
};

class GxEPD2_EPD
{
  public:
    // attributes
    const uint16_t WIDTH;
    const uint16_t HEIGHT;
    const GxEPD2::Panel panel;
    const bool hasColor;
    const bool hasPartialUpdate;
    const bool hasFastPartialUpdate;
    // constructor
    GxEPD2_EPD(int8_t cs, int8_t dc, int8_t rst, int8_t busy, int8_t busy_level, uint32_t busy_timeout,
               uint16_t w, uint16_t h, GxEPD2::Panel p, bool c, bool pu, bool fpu);
    virtual void init(uint32_t serial_diag_bitrate = 0) = 0; // serial_diag_bitrate = 0 : disabled
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    virtual void clearScreen(uint8_t value) = 0; // init controller memory and screen (default white)
    virtual void writeScreenBuffer(uint8_t value) = 0; // init controller memory (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    virtual void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false) = 0;
    virtual void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false) = 0;
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    virtual void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false) = 0;
    virtual void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false) = 0;
    virtual void refresh(bool partial_update_mode = false) = 0; // screen refresh from controller memory to full screen
    virtual void refresh(int16_t x, int16_t y, int16_t w, int16_t h) = 0; // screen refresh from controller memory, partial screen
    virtual void setPaged() {}; // for GxEPD2_154c paged workaround
    static inline uint16_t gx_uint16_min(uint16_t a, uint16_t b)
    {
      return (a < b ? a : b);
    };
    static inline uint16_t gx_uint16_max(uint16_t a, uint16_t b)
    {
      return (a > b ? a : b);
    };
  protected:
    void _waitWhileBusy(const char* comment = 0, uint16_t busy_time = 5000);
    void _writeCommand(uint8_t c);
    void _writeData(uint8_t d);
    void _writeData(const uint8_t* data, uint16_t n);
    void _writeDataPGM(const uint8_t* data, uint16_t n);
    void _writeCommandData(const uint8_t* pCommandData, uint8_t datalen);
    void _writeCommandDataPGM(const uint8_t* pCommandData, uint8_t datalen);
  protected:
    int8_t _cs, _dc, _rst, _busy, _busy_level;
    uint32_t _busy_timeout;
    bool _diag_enabled;
    SPISettings _spi_settings;
};

#endif

