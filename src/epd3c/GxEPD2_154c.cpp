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

#include "GxEPD2_154c.h"
#include "WaveTables.h"

const uint8_t GxEPD2_154c::bw2grey[] =
{
  0b00000000, 0b00000011, 0b00001100, 0b00001111,
  0b00110000, 0b00110011, 0b00111100, 0b00111111,
  0b11000000, 0b11000011, 0b11001100, 0b11001111,
  0b11110000, 0b11110011, 0b11111100, 0b11111111,
};

GxEPD2_154c::GxEPD2_154c(int8_t cs, int8_t dc, int8_t rst, int8_t busy) : GxEPD2_EPD(cs, dc, rst, busy, LOW, 20000000)
{
  _initial = true;
  _power_is_on = false;
}

void GxEPD2_154c::init(uint32_t serial_diag_bitrate)
{
  GxEPD2_EPD::init(serial_diag_bitrate);
  _initial = true;
  _power_is_on = false;
}

void GxEPD2_154c::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_154c::clearScreen(uint8_t black_value, uint8_t color_value)
{
  _Init_Full();
  _writeCommand(0x10);
  for (int16_t i = 0; i < WIDTH * HEIGHT / 8; i++)
  {
    _writeData(bw2grey[(black_value & 0xF0) >> 4]);
    _writeData(bw2grey[black_value & 0x0F]);
  }
  _writeCommand(0x13);
  for (int16_t i = 0; i < WIDTH * HEIGHT / 8; i++)
  {
    _writeData(color_value);
  }
  _Update_Full();
  _initial = false;
}

void GxEPD2_154c::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_154c::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  _Init_Full();
  _writeCommand(0x10);
  for (int16_t i = 0; i < WIDTH * HEIGHT / 8; i++)
  {
    _writeData(bw2grey[(black_value & 0xF0) >> 4]);
    _writeData(bw2grey[black_value & 0x0F]);
  }
  _writeCommand(0x13);
  for (int16_t i = 0; i < WIDTH * HEIGHT / 8; i++)
  {
    _writeData(color_value);
  }
}

void GxEPD2_154c::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, NULL, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_154c::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  x -= x % 8; // byte boundary
  w -= x % 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < WIDTH ? w : WIDTH - x; // limit
  int16_t h1 = y + h < HEIGHT ? h : HEIGHT - y; // limit
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
        // use w, h of bitmap for index!
        //if ((i + dy < h) && (j + dx < w))
        if (((x + j) >= 0) && ((x + j) < w) && ((y + i) >= 0) && ((y + i) < h))
        {
          int16_t idx = mirror_y ? (x + j) / 8 + ((h - 1 - (y + i))) * (w / 8) : (x + j) / 8 + (y + i) * (w / 8);
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
      //_writeData(data);
      _writeData(bw2grey[(data & 0xF0) >> 4]);
      _writeData(bw2grey[data & 0x0F]);
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
        // use w, h of bitmap for index!
        //if ((i + dy < h) && (j + dx < w))
        if (((x + j) >= 0) && ((x + j) < w) && ((y + i) >= 0) && ((y + i) < h))
        {
          int16_t idx = mirror_y ? (x + j) / 8 + ((h - 1 - (y + i))) * (w / 8) : (x + j) / 8 + (y + i) * (w / 8);
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
}

void GxEPD2_154c::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_154c::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_154c::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_154c::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_154c::refresh(bool partial_update_mode)
{
  _Update_Full();
}

void GxEPD2_154c::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  _Update_Full();
}

void GxEPD2_154c::powerOff()
{
  _PowerOff();
}

void GxEPD2_154c::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
}

void GxEPD2_154c::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn");
  }
  _power_is_on = true;
}

void GxEPD2_154c::_PowerOff()
{
  _writeCommand(0x02); // power off
  _waitWhileBusy("_PowerOff");
  _power_is_on = false;
}

void GxEPD2_154c::_InitDisplay()
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
  _writeData(0x07);
  _writeData(0x00);
  _writeData(0x08);
  _writeData(0x00);
  _writeCommand(0x06);
  _writeData(0x07);
  _writeData(0x07);
  _writeData(0x07);
  _writeCommand(0x04);
  // power on needed here!
  _waitWhileBusy("Power On");
  _writeCommand(0X00);
  _writeData(0xcf);
  _writeCommand(0X50);
  _writeData(0x37);
  _writeCommand(0x30);
  _writeData(0x39);
  _writeCommand(0x61);
  _writeData(0xC8);
  _writeData(0x00);
  _writeData(0xC8);
  _writeCommand(0x82);
  _writeData(0x0E);
}

void GxEPD2_154c::_Init_Full()
{
  _InitDisplay();
  _writeCommand(0x20);
  _writeData(GxGDEW0154Z04_lut_20_vcom0, sizeof(GxGDEW0154Z04_lut_20_vcom0));
  _writeCommand(0x21);
  _writeData(GxGDEW0154Z04_lut_21_w, sizeof(GxGDEW0154Z04_lut_21_w));
  _writeCommand(0x22);
  _writeData(GxGDEW0154Z04_lut_22_b, sizeof(GxGDEW0154Z04_lut_22_b));
  _writeCommand(0x23);
  _writeData(GxGDEW0154Z04_lut_23_g1, sizeof(GxGDEW0154Z04_lut_23_g1));
  _writeCommand(0x24);
  _writeData(GxGDEW0154Z04_lut_24_g2, sizeof(GxGDEW0154Z04_lut_24_g2));
  _writeCommand(0x25);
  _writeData(GxGDEW0154Z04_lut_25_vcom1, sizeof(GxGDEW0154Z04_lut_25_vcom1));
  _writeCommand(0x26);
  _writeData(GxGDEW0154Z04_lut_26_red0, sizeof(GxGDEW0154Z04_lut_26_red0));
  _writeCommand(0x27);
  _writeData(GxGDEW0154Z04_lut_27_red1, sizeof(GxGDEW0154Z04_lut_27_red1));
  _PowerOn();
}

void GxEPD2_154c::_Init_Part()
{
  _InitDisplay();
  _PowerOn();
}

void GxEPD2_154c::_Update_Full()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full");
}

void GxEPD2_154c::_Update_Part()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part");
}



