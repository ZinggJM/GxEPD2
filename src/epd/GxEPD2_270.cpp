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

#include "GxEPD2_270.h"
#include "WaveTables.h"

GxEPD2_270::GxEPD2_270(int8_t cs, int8_t dc, int8_t rst, int8_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 10000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _initial = true;
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_270::init(uint32_t serial_diag_bitrate)
{
  GxEPD2_EPD::init(serial_diag_bitrate);
  _initial = true;
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_270::clearScreen(uint8_t value)
{
  _Init_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  _refreshWindow(0, 0, WIDTH, HEIGHT);
  _waitWhileBusy("clearScreen", full_refresh_time);
  _initial = false;
}

void GxEPD2_270::writeScreenBuffer(uint8_t value)
{
  //if (_initial) clearScreen(value);
  //else _writeScreenBuffer(value);
  clearScreen(value);
}

void GxEPD2_270::_writeScreenBuffer(uint8_t value)
{
  _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  _writeCommand(0x92); // partial out
}

void GxEPD2_270::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  int16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  if (!_using_partial_mode) _Init_Part();
  _setPartialRamArea(x1, y1, w1, h1);
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb, h of bitmap for index!
      int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[idx]);
#else
        data = bitmap[idx];
#endif
      }
      else
      {
        data = bitmap[idx];
      }
      if (invert) data = ~data;
      else _writeData(data);
    }
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_270::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    writeImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_270::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_270::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_270::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_270::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_270::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else
  {
    if (_using_partial_mode) _Init_Full();
    _Update_Full();
  }
}

void GxEPD2_270::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  x -= x % 8; // byte boundary
  w -= x % 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  w1 -= x1 - x;
  h1 -= y1 - y;
  _refreshWindow(x1, y1, w1, h1);
  _waitWhileBusy("refresh", full_refresh_time);
}

void GxEPD2_270::powerOff(void)
{
  _PowerOff();
}

void GxEPD2_270::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  w = (w + 7 + (x % 8)) & 0xfff8; // byte boundary exclusive (round up)
  _writeCommand(0x15);
  _writeData(x >> 8);
  _writeData(x & 0xf8);
  _writeData(y >> 8);
  _writeData(y & 0xff);
  _writeData(w >> 8);
  _writeData(w & 0xf8);
  _writeData(h >> 8);
  _writeData(h & 0xff);
}

void GxEPD2_270::_refreshWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  w = (w + 7 + (x % 8)) & 0xfff8; // byte boundary exclusive (round up)
  h = gx_uint16_min(h, 256); // strange controller error
  _writeCommand(0x16);
  _writeData(x >> 8);
  _writeData(x & 0xf8);
  _writeData(y >> 8);
  _writeData(y & 0xff);
  _writeData(w >> 8);
  _writeData(w & 0xf8);
  _writeData(h >> 8);
  _writeData(h & 0xff);
}

void GxEPD2_270::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_270::_PowerOff()
{
  _writeCommand(0x02); // power off
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_270::_InitDisplay()
{
  _writeCommand(0x01);
  _writeData (0x03);
  _writeData (0x00);
  _writeData (0x2b);
  _writeData (0x2b);
  _writeData (0x09);
  _writeCommand(0x06);
  _writeData (0x07);
  _writeData (0x07);
  _writeData (0x17);
  _writeCommand(0xF8);
  _writeData (0x60);
  _writeData (0xA5);
  _writeCommand(0xF8);
  _writeData (0x89);
  _writeData (0xA5);
  _writeCommand(0xF8);
  _writeData (0x90);
  _writeData (0x00);
  _writeCommand(0xF8);
  _writeData (0x93);
  _writeData (0x2A);
  _writeCommand(0xF8);
  _writeData (0xa0);
  _writeData (0xa5);
  _writeCommand(0xF8);
  _writeData (0xa1);
  _writeData (0x00);
  _writeCommand(0xF8);
  _writeData (0x73);
  _writeData (0x41);
  _writeCommand(0x16);
  _writeData(0x00);
  //_writeCommand(0x04);
  //_waitWhileBusy("_wakeUp Power On");
  _writeCommand(0x00);
  _writeData(0x9f); // b/w, by OTP LUT
  _writeCommand(0x30);
  _writeData (0x3a); //3A 100HZ
  _writeCommand(0x61);
  _writeData (0x00);
  _writeData (0xb0); //176
  _writeData (0x01);
  _writeData (0x08); //264
  _writeCommand(0x82);
  _writeData (0x12);
}

void GxEPD2_270::_Init_Full()
{
  _InitDisplay();
  _writeCommand(0x20);
  _writeDataPGM(GxGDEW027W3_lut_20_vcomDC, sizeof(GxGDEW027W3_lut_20_vcomDC));
  _writeCommand(0x21);
  _writeDataPGM(GxGDEW027W3_lut_21_ww, sizeof(GxGDEW027W3_lut_21_ww));
  _writeCommand(0x22);
  _writeDataPGM(GxGDEW027W3_lut_22_bw, sizeof(GxGDEW027W3_lut_22_bw));
  _writeCommand(0x23);
  _writeDataPGM(GxGDEW027W3_lut_23_wb, sizeof(GxGDEW027W3_lut_23_wb));
  _writeCommand(0x24);
  _writeDataPGM(GxGDEW027W3_lut_24_bb, sizeof(GxGDEW027W3_lut_24_bb));
  _PowerOn();
  _using_partial_mode = false;
}

void GxEPD2_270::_Init_Part()
{
  _InitDisplay();
  // no partial update LUT
  _writeCommand(0x20);
  _writeDataPGM(GxGDEW027W3_lut_20_vcomDC, sizeof(GxGDEW027W3_lut_20_vcomDC));
  _writeCommand(0x21);
  _writeDataPGM(GxGDEW027W3_lut_21_ww, sizeof(GxGDEW027W3_lut_21_ww));
  _writeCommand(0x22);
  _writeDataPGM(GxGDEW027W3_lut_22_bw, sizeof(GxGDEW027W3_lut_22_bw));
  _writeCommand(0x23);
  _writeDataPGM(GxGDEW027W3_lut_23_wb, sizeof(GxGDEW027W3_lut_23_wb));
  _writeCommand(0x24);
  _writeDataPGM(GxGDEW027W3_lut_24_bb, sizeof(GxGDEW027W3_lut_24_bb));
  _PowerOn();
  _using_partial_mode = true;
}

void GxEPD2_270::_Update_Full()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_270::_Update_Part()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part", full_refresh_time);
}


