// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Controller: IL0398 : http://www.e-paper-display.com/download_detail/downloadsId=537.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_420c_V2.h"

GxEPD2_420c_V2::GxEPD2_420c_V2(int8_t cs, int8_t dc, int8_t rst, int8_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 20000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _pulldown_rst_mode = false;
  _reset_duration = 2;
}

void GxEPD2_420c_V2::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_420c_V2::clearScreen(uint8_t black_value, uint8_t color_value)
{
  writeScreenBuffer(black_value, color_value);

  _Update_Full();
}

void GxEPD2_420c_V2::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_420c_V2::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  uint32_t Width, Height;
  Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);

  _Init_Full();

  _writeCommand(0x10);
  _startTransfer();
  for (uint32_t j = 0; j < HEIGHT; j++) 
  {
    for (uint32_t i = 0; i < Width; i++) 
    {
      _transfer(black_value);
    }
  }
  _endTransfer();

  _writeCommand(0x13);
  _startTransfer();
  for (uint32_t j = 0; j < Height; j++) 
  {
    for (uint32_t i = 0; i < Width; i++) 
    {
      _transfer(color_value);
    }
  }
  _endTransfer();
}

void GxEPD2_420c_V2::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, NULL, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_420c_V2::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImage("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32

  int16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  if ((w <= 0) || (h <= 0)) return;
  _Init_Full();
  _writeCommand(0x10);
  for (int16_t i = 0; i < HEIGHT; i++)
  {
    for (int16_t j = 0; j < WIDTH; j += 8)
    {
      uint8_t data = 0xFF;
      if (black)
      {
        if ((j >= x) && (j <= x + w) && (i >= y) && (i < y + h))
        {
          int16_t idx = mirror_y ? (j - x) / 8 + ((h - 1 - (i - y))) * wb : (j - x) / 8 + (i - y) * wb;
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
      }
      _writeData(data);
    }
  }
  _writeCommand(0x13);
  for (int16_t i = 0; i < HEIGHT; i++)
  {
    for (int16_t j = 0; j < WIDTH; j += 8)
    {
      uint8_t data = 0xFF;
      if (color)
      {
        if ((j >= x) && (j <= x + w) && (i >= y) && (i < y + h))
        {
          int16_t idx = mirror_y ? (j - x) / 8 + ((h - 1 - (i - y))) * wb : (j - x) / 8 + (i - y) * wb;
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
      }
      _writeData(data);
    }
  }
  
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_420c_V2::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                 int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, NULL, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_420c_V2::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                 int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wb_bitmap = (w_bitmap + 7) / 8; // width bytes, bitmaps are padded
  x_part -= x_part % 8; // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w; // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h; // limit
  x -= x % 8; // byte boundary
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
  _Init_Full();
  _writeCommand(0x10);
  for (int16_t i = 0; i < HEIGHT; i++)
  {
    for (int16_t j = 0; j < WIDTH; j += 8)
    {
      uint8_t data = 0xFF;
      if (black)
      {
        if ((j >= x1) && (j <= x1 + w) && (i >= y1) && (i < y1 + h))
        {
          // use wb_bitmap, h_bitmap of bitmap for index!
          //int16_t idx = mirror_y ? x_part / 8 + j + dx / 8 + ((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + (y_part + i + dy) * wb_bitmap;
          int16_t idx = mirror_y ? (x_part + j - x1) / 8 + ((h_bitmap - 1 - (y_part + i - y1))) * wb_bitmap : (x_part + j - x1) / 8 + (y_part + i - y1) * wb_bitmap;
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
      }
      _writeData(data);
    }
  }
  _writeCommand(0x13);
  for (int16_t i = 0; i < HEIGHT; i++)
  {
    for (int16_t j = 0; j < WIDTH; j += 8)
    {
      uint8_t data = 0xFF;
      if (color)
      {
        if ((j >= x1) && (j <= x1 + w) && (i >= y1) && (i < y1 + h))
        {
          // use wb_bitmap, h_bitmap of bitmap for index!
          int16_t idx = mirror_y ? (x_part + j - x1) / 8 + ((h_bitmap - 1 - (y_part + i - y1))) * wb_bitmap : (x_part + j - x1) / 8 + (y_part + i - y1) * wb_bitmap;
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
      }
      _writeData(data);
    }
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_420c_V2::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_420c_V2::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_V2::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_V2::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_V2::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_V2::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_V2::refresh(bool partial_update_mode)
{
  _Update_Full();
}

void GxEPD2_420c_V2::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  _Update_Full();
}

void GxEPD2_420c_V2::powerOff()
{
  _PowerOff();
}

void GxEPD2_420c_V2::hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x07); // deep sleep
    _writeData(0xA5);    // check code
    _hibernating = true;
  }
}

void GxEPD2_420c_V2::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
}

void GxEPD2_420c_V2::_PowerOn()
{
  Serial.println("GxEPD2_420c_V2::_PowerOn()");
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_420c_V2::_PowerOff()
{
  _writeCommand(0x02); // power off
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
}


void GxEPD2_420c_V2::_InitDisplay()
{
  Serial.println("GxEPD2_420c_V2::_InitDisplay()");
  if (_hibernating) _reset();
  _PowerOn();
  _writeCommand(0x00);
  _writeData(0x0F);
}

void GxEPD2_420c_V2::_Init_Full()
{
  _InitDisplay();
  _PowerOn();
}

void GxEPD2_420c_V2::_Init_Part()
{
  _InitDisplay();
  _PowerOn();
}

void GxEPD2_420c_V2::_Update_Full()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_420c_V2::_Update_Part()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}
