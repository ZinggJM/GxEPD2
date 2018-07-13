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

#include "GxEPD2_270c.h"
#include "WaveTables.h"

GxEPD2_270c::GxEPD2_270c(int8_t cs, int8_t dc, int8_t rst, int8_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 20000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _initial = true;
  _power_is_on = false;
}

void GxEPD2_270c::init(uint32_t serial_diag_bitrate)
{
  GxEPD2_EPD::init(serial_diag_bitrate);
  _initial = true;
  _power_is_on = false;
}

void GxEPD2_270c::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_270c::clearScreen(uint8_t black_value, uint8_t red_value)
{
  _Init_Part();
  _setPartialRamArea_270c(0x14, 0, 0, WIDTH, HEIGHT);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(~black_value);
  }
  _setPartialRamArea_270c(0x15, 0, 0, WIDTH, HEIGHT);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(~red_value);
  }
  refresh(0, 0, WIDTH, HEIGHT);
}

void GxEPD2_270c::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_270c::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  _Init_Part();
  _setPartialRamArea_270c(0x14, 0, 0, WIDTH, HEIGHT);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(~black_value);
  }
  _setPartialRamArea_270c(0x15, 0, 0, WIDTH, HEIGHT);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(~color_value);
  }
}

void GxEPD2_270c::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, NULL, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_270c::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
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
  _Init_Part();
  _setPartialRamArea_270c(0x14, x1, y1, w1, h1);
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (black)
      {
        // use wb, h of bitmap for index!
        int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
        if (pgm)
        {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
          data = pgm_read_byte(&black[idx]);
#else
          data = black[idx];
#endif
        }
        else
        {
          data = black[idx];
        }
        if (invert) data = ~data;
      }
      _writeData(~data);
    }
  }
  _setPartialRamArea_270c(0x15, x1, y1, w1, h1);
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (color)
      {
        // use wb, h of bitmap for index!
        int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
        if (pgm)
        {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
          data = pgm_read_byte(&color[idx]);
#else
          data = color[idx];
#endif
        }
        else
        {
          data = color[idx];
        }
        if (invert) data = ~data;
      }
      _writeData(~data);
    }
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_270c::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_270c::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_270c::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_270c::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_270c::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else _Update_Full();
}

void GxEPD2_270c::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  x -= x % 8; // byte boundary
  w -= x % 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  w1 -= x1 - x;
  h1 -= y1 - y;
  //_refreshWindow(x1, y1, w1, h1);
  w1 = (w1 + 7 + (x1 % 8)) & 0xfff8; // byte boundary exclusive (round up)
  h1 = gx_uint16_min(h1, 256); // strange controller error
  _writeCommand(0x16);
  _writeData(x1 >> 8);
  _writeData(x1 & 0xf8);
  _writeData(y1 >> 8);
  _writeData(y1 & 0xff);
  _writeData(w1 >> 8);
  _writeData(w1 & 0xf8);
  _writeData(h1 >> 8);
  _writeData(h1 & 0xff);
  _waitWhileBusy("refresh", partial_refresh_time);
}

void GxEPD2_270c::powerOff()
{
  _PowerOff();
}

void GxEPD2_270c::_writeData_nCS(const uint8_t* data, uint16_t n)
{
  SPI.beginTransaction(_spi_settings);
  for (uint8_t i = 0; i < n; i++)
  {
    if (_cs >= 0) digitalWrite(_cs, LOW);
    SPI.transfer(pgm_read_byte(&*data++));
    if (_cs >= 0) digitalWrite(_cs, HIGH);
  }
  SPI.endTransaction();
}

void GxEPD2_270c::_setPartialRamArea_270c(uint8_t cmd, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  w = (w + 7 + (x % 8)) & 0xfff8; // byte boundary exclusive (round up)
  _writeCommand(cmd);
  _writeData(x >> 8);
  _writeData(x & 0xf8);
  _writeData(y >> 8);
  _writeData(y & 0xff);
  _writeData(w >> 8);
  _writeData(w & 0xf8);
  _writeData(h >> 8);
  _writeData(h & 0xff);
}

void GxEPD2_270c::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_270c::_PowerOff()
{
  _writeCommand(0x02); // power off
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
}

void GxEPD2_270c::_InitDisplay()
{
  // reset required for wakeup
  if (!_power_is_on && (_rst >= 0))
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }
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
  _writeData (0x73);
  _writeData (0x41);
  _writeCommand(0x16);
  _writeData(0x00);
  //_writeCommand(0x04);
  //_waitWhileBusy("_wakeUp Power On");
  _writeCommand(0x00);
  _writeData(0xaf); // by register LUT
  _writeCommand(0x30);
  _writeData (0x3a);
  _writeCommand(0x61);
  _writeData (0x00);
  _writeData (0xb0); //176
  _writeData (0x01);
  _writeData (0x08); //264
  _writeCommand(0x82);
  _writeData (0x12);
  _writeCommand(0X50);
  _writeData(0x87);
}

void GxEPD2_270c::_Init_Full()
{
  _InitDisplay();
  _writeCommand(0x20); //vcom
  _writeData_nCS(GxGDEW027C44_lut_20_vcomDC, sizeof(GxGDEW027C44_lut_20_vcomDC));
  _writeCommand(0x21); //ww --
  _writeData_nCS(GxGDEW027C44_lut_21, sizeof(GxGDEW027C44_lut_21));
  _writeCommand(0x22); //bw r
  _writeData_nCS(GxGDEW027C44_lut_22_red, sizeof(GxGDEW027C44_lut_22_red));
  _writeCommand(0x23); //wb w
  _writeData_nCS(GxGDEW027C44_lut_23_white, sizeof(GxGDEW027C44_lut_23_white));
  _writeCommand(0x24); //bb b
  _writeData_nCS(GxGDEW027C44_lut_24_black, sizeof(GxGDEW027C44_lut_24_black));
  _PowerOn();
}

void GxEPD2_270c::_Init_Part()
{
  _InitDisplay();
  _writeCommand(0x20); //vcom
  _writeData_nCS(GxGDEW027C44_lut_20_vcomDC, sizeof(GxGDEW027C44_lut_20_vcomDC));
  _writeCommand(0x21); //ww --
  _writeData_nCS(GxGDEW027C44_lut_21, sizeof(GxGDEW027C44_lut_21));
  _writeCommand(0x22); //bw r
  _writeData_nCS(GxGDEW027C44_lut_22_red, sizeof(GxGDEW027C44_lut_22_red));
  _writeCommand(0x23); //wb w
  _writeData_nCS(GxGDEW027C44_lut_23_white, sizeof(GxGDEW027C44_lut_23_white));
  _writeCommand(0x24); //bb b
  _writeData_nCS(GxGDEW027C44_lut_24_black, sizeof(GxGDEW027C44_lut_24_black));
  _PowerOn();
}

void GxEPD2_270c::_Update_Full()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_270c::_Update_Part()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}



