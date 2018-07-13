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

#include "GxEPD2_420.h"
#include "WaveTables.h"

GxEPD2_420::GxEPD2_420(int8_t cs, int8_t dc, int8_t rst, int8_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 10000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _initial = true;
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_420::init(uint32_t serial_diag_bitrate)
{
  GxEPD2_EPD::init(serial_diag_bitrate);
  _initial = true;
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_420::clearScreen(uint8_t value)
{
  if (_initial)
  {
    _Init_Full();
    _writeCommand(0x13);
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
      _writeData(value);
    }
    _Update_Full();
    _initial = false;
  }
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x13);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  _Update_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x13);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  _Update_Part();
  _writeCommand(0x92); // partial out
  _initial = false;
}

void GxEPD2_420::writeScreenBuffer(uint8_t value)
{
  if (_initial) clearScreen(value);
  else _writeScreenBuffer(value);
}

void GxEPD2_420::_writeScreenBuffer(uint8_t value)
{
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x13);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  _writeCommand(0x92); // partial out
}

void GxEPD2_420::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
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
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x13);
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
      _writeData(data);
    }
  }
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_420::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    writeImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_420::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_420::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else
  {
    if (_using_partial_mode) _Init_Full();
    _Update_Full();
  }
}

void GxEPD2_420::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  x -= x % 8; // byte boundary
  w -= x % 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  w1 -= x1 - x;
  h1 -= y1 - y;
  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
  _writeCommand(0x92); // partial out
}

void GxEPD2_420::powerOff(void)
{
  _PowerOff();
}

void GxEPD2_420::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = (x + w - 1) | 0x0007; // byte boundary inclusive (last byte)
  uint16_t ye = y + h - 1;
  x &= 0xFFF8; // byte boundary
  _writeCommand(0x90); // partial window
  _writeData(x / 256);
  _writeData(x % 256);
  _writeData(xe / 256);
  _writeData(xe % 256);
  _writeData(y / 256);
  _writeData(y % 256);
  _writeData(ye / 256);
  _writeData(ye % 256);
  _writeData(0x01); // don't see any difference
  //_writeData(0x00); // don't see any difference
}

void GxEPD2_420::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_420::_PowerOff()
{
  _writeCommand(0x02); // power off
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_420::_InitDisplay()
{
  _writeCommand(0x06); // boost
  _writeData(0x17);
  _writeData(0x17);
  _writeData(0x17);
  _writeCommand(0x00);
  //_writeData(0x1f); // LUT from OTP Pixel with B/W.
  _writeData(0x3F); //300x400 B/W mode, LUT set by register
}

void GxEPD2_420::_Init_Full()
{
  _InitDisplay();
  _writeCommand(0x20);
  _writeDataPGM(GxGDEW042T2_lut_20_vcom0_full, sizeof(GxGDEW042T2_lut_20_vcom0_full));
  _writeCommand(0x21);
  _writeDataPGM(GxGDEW042T2_lut_21_ww_full, sizeof(GxGDEW042T2_lut_21_ww_full));
  _writeCommand(0x22);
  _writeDataPGM(GxGDEW042T2_lut_22_bw_full, sizeof(GxGDEW042T2_lut_22_bw_full));
  _writeCommand(0x23);
  _writeDataPGM(GxGDEW042T2_lut_23_wb_full, sizeof(GxGDEW042T2_lut_23_wb_full));
  _writeCommand(0x24);
  _writeDataPGM(GxGDEW042T2_lut_24_bb_full, sizeof(GxGDEW042T2_lut_24_bb_full));
  _PowerOn();
  _using_partial_mode = false;
}

void GxEPD2_420::_Init_Part()
{
  _InitDisplay();
  _writeCommand(0x20);
  _writeDataPGM(GxGDEW042T2_lut_20_vcom0_partial, sizeof(GxGDEW042T2_lut_20_vcom0_partial));
  _writeCommand(0x21);
  _writeDataPGM(GxGDEW042T2_lut_21_ww_partial, sizeof(GxGDEW042T2_lut_21_ww_partial));
  _writeCommand(0x22);
  _writeDataPGM(GxGDEW042T2_lut_22_bw_partial, sizeof(GxGDEW042T2_lut_22_bw_partial));
  _writeCommand(0x23);
  _writeDataPGM(GxGDEW042T2_lut_23_wb_partial, sizeof(GxGDEW042T2_lut_23_wb_partial));
  _writeCommand(0x24);
  _writeDataPGM(GxGDEW042T2_lut_24_bb_partial, sizeof(GxGDEW042T2_lut_24_bb_partial));
  _PowerOn();
  _using_partial_mode = true;
}

void GxEPD2_420::_Update_Full()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_420::_Update_Part()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}


