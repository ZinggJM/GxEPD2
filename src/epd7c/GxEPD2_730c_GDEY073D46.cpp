// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: https://www.good-display.com/product/442.html
// Panel: GDEY073D46 : https://www.good-display.com/product/442.html
// Controller: unknown
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_730c_GDEY073D46.h"

GxEPD2_730c_GDEY073D46::GxEPD2_730c_GDEY073D46(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 60000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}

void GxEPD2_730c_GDEY073D46::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_730c_GDEY073D46::clearScreen(uint8_t black_value, uint8_t color_value)
{
  writeScreenBuffer(black_value, color_value);
  refresh();
}

void GxEPD2_730c_GDEY073D46::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_730c_GDEY073D46::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  if (!_init_display_done) _InitDisplay();
  _writeCommand(0x10);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 2; i++)
  {
    //_transfer(0x11);
    _transfer(0xFF == black_value ? 0x11 : black_value);
  }
  _endTransfer();
}

void GxEPD2_730c_GDEY073D46::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImage("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
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
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
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
      for (int16_t k = 0; k < 4; k++)
      {
        uint8_t data2 = (data & 0x80 ? 0x10 : 0x00) | (data & 0x40 ? 0x01 : 0x00);
        data <<= 2;
        _transfer(data2);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_730c_GDEY073D46::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!black && !color) return;
  if (!color) return writeImage(black, x, y, w, h, invert, mirror_y, pgm);
  //Serial.print("writeImage_3c("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
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
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t black_data = 0xFF, color_data = 0xFF;
      // use wb, h of bitmap for index!
      uint32_t idx = mirror_y ? j + dx / 8 + uint32_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint32_t(i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        black_data = pgm_read_byte(&black[idx]);
        color_data = pgm_read_byte(&color[idx]);
#else
        black_data = black[idx];
        color_data = color[idx];
#endif
      }
      else
      {
        black_data = black[idx];
        color_data = color[idx];
      }
      if (invert)
      {
        black_data = ~black_data;
        color_data = ~color_data;
      }
      for (int16_t k = 0; k < 4; k++)
      {
        uint8_t out_data = 0x00;
        for (int16_t l = 0; l < 2; l++)
        {
          out_data <<= 4;
          if (!(color_data & 0x80)) out_data |= 0x04;
          else out_data |= black_data & 0x80 ? 0x01 : 0x00;
          black_data <<= 1;
          color_data <<= 1;
        }
        _transfer(out_data);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_730c_GDEY073D46::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImagePart("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", ");
  //Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wb_bitmap = (w_bitmap + 7) / 8; // width bytes, bitmaps are padded
  x_part -= x_part % 8; // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w; // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h; // limit
  // make x, w multiple of 8
  w += x % 8; // adjust for byte boundary of x
  x -= x % 8; // byte boundary ox
  w = 8 * ((w + 7) / 8); // byte boundary, bitmaps are padded
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb_bitmap, h_bitmap of bitmap for index!
      uint32_t idx = mirror_y ? x_part / 8 + j + dx / 8 + uint32_t((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + uint32_t(y_part + i + dy) * wb_bitmap;
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
      for (int16_t k = 0; k < 4; k++)
      {
        uint8_t data2 = (data & 0x80 ? 0x10 : 0x00) | (data & 0x40 ? 0x01 : 0x00);
        data <<= 2;
        _transfer(data2);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_730c_GDEY073D46::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImagePart_3c("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", ");
  //Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  if (!black && !color) return;
  if (!color) return writeImagePart(black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wb_bitmap = (w_bitmap + 7) / 8; // width bytes, bitmaps are padded
  x_part -= x_part % 8; // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w; // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h; // limit
  // make x, w multiple of 8
  w += x % 8; // adjust for byte boundary of x
  x -= x % 8; // byte boundary ox
  w = 8 * ((w + 7) / 8); // byte boundary, bitmaps are padded
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t black_data = 0xFF, color_data = 0xFF;
      // use wb_bitmap, h_bitmap of bitmap for index!
      uint32_t idx = mirror_y ? x_part / 8 + j + dx / 8 + uint32_t((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + uint32_t(y_part + i + dy) * wb_bitmap;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        black_data = pgm_read_byte(&black[idx]);
        color_data = pgm_read_byte(&color[idx]);
#else
        black_data = black[idx];
        color_data = color[idx];
#endif
      }
      else
      {
        black_data = black[idx];
        color_data = color[idx];
      }
      if (invert)
      {
        black_data = ~black_data;
        color_data = ~color_data;
      }
      for (int16_t k = 0; k < 4; k++)
      {
        uint8_t out_data = 0x00;
        for (int16_t l = 0; l < 2; l++)
        {
          out_data <<= 4;
          if (!(color_data & 0x80)) out_data |= 0x04;
          else out_data |= black_data & 0x80 ? 0x01 : 0x00;
          black_data <<= 1;
          color_data <<= 1;
        }
        _transfer(out_data);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_730c_GDEY073D46::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!data1) return;
  //Serial.print("writeNative("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  uint32_t wb = (w + 1) / 2; // width bytes, bitmaps are padded
  x -= x % 2; // byte boundary
  w = wb * 2; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 2; j++)
    {
      uint8_t data;
      // use wb, h of bitmap for index!
      uint32_t idx = mirror_y ? j + dx / 2 + uint32_t((h - 1 - (i + dy))) * wb : j + dx / 2 + uint32_t(i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&data1[idx]);
#else
        data = data1[idx];
#endif
      }
      else
      {
        data = data1[idx];
      }
      if (invert) data = ~data;
      _transfer(data);
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_730c_GDEY073D46::writeNativePart(const uint8_t* data1, const uint8_t* data2, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!data1) return;
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wb_bitmap = (w_bitmap + 1) / 2; // width bytes, bitmaps are padded
  x_part -= x_part % 2; // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w; // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h; // limit
  w += x % 2; // adjust for byte boundary of x
  x -= x % 2; // byte boundary
  w = 2 * ((w + 1) / 2); // byte boundary, bitmaps are padded
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 2; j++)
    {
      uint8_t data;
      // use wb_bitmap, h_bitmap of bitmap for index!
      uint32_t idx = mirror_y ? x_part / 2 + j + dx / 2 + uint32_t((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 2 + j + dx / 2 + uint32_t(y_part + i + dy) * wb_bitmap;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&data1[idx]);
#else
        data = data1[idx];
#endif
      }
      else
      {
        data = data1[idx];
      }
      if (invert) data = ~data;
      _transfer(data);
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_730c_GDEY073D46::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_730c_GDEY073D46::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_730c_GDEY073D46::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_730c_GDEY073D46::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_730c_GDEY073D46::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_730c_GDEY073D46::refresh(bool partial_update_mode)
{
  _PowerOn();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x50);
  _writeData(0x3F);
  _writeCommand(0x12); // Display Refresh
  _writeData(0x00);
  delay(1);
  _waitWhileBusy("_refresh", full_refresh_time);
}

void GxEPD2_730c_GDEY073D46::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  _PowerOn();
  if (usePartialUpdate) _setPartialRamArea(x, y, w, h);
  else _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x50);
  _writeData(0x3F); // border white, no floating?
  _writeCommand(0x12); // Display Refresh
  _writeData(0x00);
  delay(1);
  _waitWhileBusy("_refresh", full_refresh_time);
}

void GxEPD2_730c_GDEY073D46::powerOff()
{
  _PowerOff();
}

void GxEPD2_730c_GDEY073D46::hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x07); // deep sleep
    _writeData(0xA5);    // control code
    _hibernating = true;
    _init_display_done = false;
  }
}

void GxEPD2_730c_GDEY073D46::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  //Serial.print("_setPartialRamArea("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h; // controller issue (one more)
  _writeCommand(0x83); // partial window
  _writeData(x / 256);
  _writeData(x % 256);
  _writeData(xe / 256);
  _writeData(xe % 256);
  _writeData(y / 256);
  _writeData(y % 256);
  _writeData(ye / 256);
  _writeData(ye % 256);
  _writeData(0x01);
}

void GxEPD2_730c_GDEY073D46::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_730c_GDEY073D46::_PowerOff()
{
  if (_power_is_on)
  {
    _writeCommand(0x02);
    _writeData(0x00);
    _waitWhileBusy("_PowerOff", power_off_time);
  }
  _power_is_on = false;
}

void GxEPD2_730c_GDEY073D46::_InitDisplay()
{
  if ((_rst >= 0) && (_hibernating || _initial_write))
  {
    pinMode(_rst, OUTPUT); // just in case
    digitalWrite(_rst, HIGH);
    delay(50); // needs a little longer
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(2);
    _waitWhileBusy("_InitDisplay reset", power_on_time);
    _initial_write = false; // used for initial reset done
    _hibernating = false;
    _power_is_on = false;
  }
  _writeCommand(0xAA); // CMDH
  _writeData(0x49);
  _writeData(0x55);
  _writeData(0x20);
  _writeData(0x08);
  _writeData(0x09);
  _writeData(0x18);
  _writeCommand(0x01); // PWRR
  _writeData(0x3F);
  _writeData(0x00);
  _writeData(0x32);
  _writeData(0x2A);
  _writeData(0x0E);
  _writeData(0x2A);
  _writeCommand(0x00); // PSR
  _writeData(0x5F);
  _writeData(0x69);
  _writeCommand(0x03); // POFS
  _writeData(0x00);
  _writeData(0x54);
  _writeData(0x00);
  _writeData(0x44);
  _writeCommand(0x05); // BTST1
  _writeData(0x40);
  _writeData(0x1F);
  _writeData(0x1F);
  _writeData(0x2C);
  _writeCommand(0x06); // BTST2
  _writeData(0x6F);
  _writeData(0x1F);
  _writeData(0x16);
  _writeData(0x25);
  _writeCommand(0x08); // BTST3
  _writeData(0x6F);
  _writeData(0x1F);
  _writeData(0x1F);
  _writeData(0x22);
  _writeCommand(0x13); // IPC
  _writeData(0x00);
  _writeData(0x04);
  _writeCommand(0x30); // PLL
  _writeData(0x02);
  _writeCommand(0x41); // TSE
  _writeData(0x00);
  _writeCommand(0x50); // CDI
  _writeData(0x3F);
  _writeCommand(0x60); // TCON
  _writeData(0x02);
  _writeData(0x00);
  _writeCommand(0x61); // TRES
  _writeData(0x03);
  _writeData(0x20);
  _writeData(0x01);
  _writeData(0xE0);
  _writeCommand(0x82); // VDCS
  _writeData(0x1E);
  _writeCommand(0x84); // T_VDCS
  _writeData(0x00);
  _writeCommand(0x86); // AGID
  _writeData(0x00);
  _writeCommand(0xE3); // PWS
  _writeData(0x2F);
  _writeCommand(0xE0); // CCSET
  _writeData(0x00);
  _writeCommand(0xE6); // TSSET
  _writeData(0x00);
  _PowerOn();
  _init_display_done = true;
}

uint8_t GxEPD2_730c_GDEY073D46::_colorOfDemoBitmap(uint8_t from, int16_t mode)
{
  switch (mode)
  {
    case 0:
      {
        switch (from)
        {
          case 0xFF: return (0x01); // white;
          case 0xFC: return (0x05); // yellow;
          case 0xF1: return (0x06); // orange;
          case 0xE5: return (0x04); // red;
          case 0x4B: return (0x03); // blue;
          case 0x39: return (0x02); // green;
          case 0x00: return (0x00); // black;
          default: return (0x01); // white;
        }
      }
    case 1:
      {
        switch (from)
        {
          case 0x00: return (0x00); // black;
          case 0xff: return (0x01); // white;
          case 0xfc: return (0x05); // yellow;
          case 0xE0: return (0x04); // red;
          case 0x03: return (0x03); // blue;
          case 0x1c: return (0x02); // green;
          default: return (0x01); // white;
        }
      }
    default: return (0x01); // white;
  }
}

void GxEPD2_730c_GDEY073D46::writeDemoBitmap(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, int16_t mode, bool mirror_y, bool pgm)
{
  (void) mode;
  if (!data1) return;
  //Serial.print("writeDemoBitmap("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1; j += 2)
    {
      uint8_t data = 0x11;
      uint32_t idx = mirror_y ? j + dx + uint32_t((h - 1 - (i + dy))) * w : j + dx + uint32_t(i + dy) * w;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = _colorOfDemoBitmap(pgm_read_byte(&data1[idx++]), mode) << 4;
        data |= _colorOfDemoBitmap(pgm_read_byte(&data1[idx]), mode);
#else
        data = _colorOfDemoBitmap(data1[idx++], mode) << 4;
        data |= _colorOfDemoBitmap(data1[idx], mode);
#endif
      }
      else
      {
        data = _colorOfDemoBitmap(data1[idx++], mode) << 4;
        data |= _colorOfDemoBitmap(data1[idx], mode);
      }
      _transfer(data);
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_730c_GDEY073D46::drawDemoBitmap(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, int16_t mode, bool mirror_y, bool pgm)
{
  writeDemoBitmap(data1, data2, x, y, w, h, mode, mirror_y, pgm);
  refresh(x, y, w, h);
}
