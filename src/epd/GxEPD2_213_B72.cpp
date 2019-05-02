// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Controller: IL3897 :
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_213_B72.h"

GxEPD2_213_B72::GxEPD2_213_B72(int8_t cs, int8_t dc, int8_t rst, int8_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, HIGH, 10000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}

void GxEPD2_213_B72::clearScreen(uint8_t value)
{
  if (_initial)
  {
    _Init_Full();
    _writeCommand(0x26); // preset previous
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
      _writeData(0xFF); // 0xFF is white
    }
    _writeCommand(0x24); // set current
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
      _writeData(value);
    }
    _Update_Full();
    _initial = false;
  }
  if (!_using_partial_mode) _Init_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x24); // set current
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  _Update_Part();
  _writeCommand(0x26); // set previous same
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
}

void GxEPD2_213_B72::writeScreenBuffer(uint8_t value)
{
  if (_initial) clearScreen(value);
  else _writeScreenBuffer(value);
}

void GxEPD2_213_B72::_writeScreenBuffer(uint8_t value)
{
  if (!_using_partial_mode) _Init_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x24);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
}

void GxEPD2_213_B72::writeScreenBufferAgain(uint8_t value)
{
  if (!_using_partial_mode) _Init_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x26);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
}

void GxEPD2_213_B72::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImage(0x24, bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_213_B72::writeImageAgain(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImage(0x26, bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_213_B72::_writeImage(uint8_t command, const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
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
  _writeCommand(command);
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

void GxEPD2_213_B72::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    writeImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_213_B72::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_213_B72::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
  writeImageAgain(bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_213_B72::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    drawImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_213_B72::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    drawImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_213_B72::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else
  {
    if (_using_partial_mode) _Init_Full();
    _Update_Full();
  }
}

void GxEPD2_213_B72::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
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
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
}

void GxEPD2_213_B72::powerOff(void)
{
  _PowerOff();
}

void GxEPD2_213_B72::hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x10); // deep sleep mode
    _writeData(0x1);     // enter deep sleep
    _hibernating = true;
  }
}

void GxEPD2_213_B72::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  _writeCommand(0x11); // set ram entry mode
  _writeData(0x03);    // x increase, y increase : normal mode
  _writeCommand(0x44);
  _writeData(x / 8);
  _writeData((x + w - 1) / 8);
  _writeCommand(0x45);
  _writeData(y % 256);
  _writeData(y / 256);
  _writeData((y + h - 1) % 256);
  _writeData((y + h - 1) / 256);
  _writeCommand(0x4e);
  _writeData(x / 8);
  _writeCommand(0x4f);
  _writeData(y % 256);
  _writeData(y / 256);
}

void GxEPD2_213_B72::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x22);
    _writeData(0xc0);
    _writeCommand(0x20);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_213_B72::_PowerOff()
{
  _writeCommand(0x22);
  _writeData(0xc3);
  _writeCommand(0x20);
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_213_B72::_InitDisplay()
{
  if (_hibernating) _reset();
  _writeCommand(0x74); //set analog block control
  _writeData(0x54);
  _writeCommand(0x7E); //set digital block control
  _writeData(0x3B);
  _writeCommand(0x01); //Driver output control
  _writeData(0xF9);    // (HEIGHT - 1) % 256
  _writeData(0x00);    // (HEIGHT - 1) / 256
  _writeData(0x00);
  _writeCommand(0x3C); //BorderWavefrom
  _writeData(0x03);
  _writeCommand(0x2C); //VCOM Voltage
  _writeData(0x70);    // NA ??
  _writeCommand(0x03); //Gate Driving voltage Control
  _writeData(0x15);    // 19V
  _writeCommand(0x04); //Source Driving voltage Control
  _writeData(0x41);    // VSH1 15V
  _writeData(0xA8);    // VSH2 5V
  _writeData(0x32);    // VSL -15V
  _writeCommand(0x3A); //Dummy Line
  _writeData(0x30);
  _writeCommand(0x3B); //Gate time
  _writeData(0x0A);
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
}

const uint8_t GxEPD2_213_B72::LUT_DATA_full[] PROGMEM =
{
  0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, //LUT0: BB:     VS 0 ~7
  0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, //LUT1: BW:     VS 0 ~7
  0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, //LUT2: WB:     VS 0 ~7
  0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, //LUT3: WW:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT4: VCOM:   VS 0 ~7
  0x03, 0x03, 0x00, 0x00, 0x02, // TP0 A~D RP0
  0x09, 0x09, 0x00, 0x00, 0x02, // TP1 A~D RP1
  0x03, 0x03, 0x00, 0x00, 0x02, // TP2 A~D RP2
  0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
  0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
  0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
  0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6
};

const uint8_t GxEPD2_213_B72::LUT_DATA_part[] PROGMEM =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT0: BB:     VS 0 ~7
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT1: BW:     VS 0 ~7
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT2: WB:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT3: WW:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT4: VCOM:   VS 0 ~7
  0x0A, 0x00, 0x00, 0x00, 0x00, // TP0 A~D RP0
  0x00, 0x00, 0x00, 0x00, 0x00, // TP1 A~D RP1
  0x00, 0x00, 0x00, 0x00, 0x00, // TP2 A~D RP2
  0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
  0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
  0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
  0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6
};

void GxEPD2_213_B72::_Init_Full()
{
  _InitDisplay();
  _writeCommand(0x32);
  _writeDataPGM(LUT_DATA_full, sizeof(LUT_DATA_full));
  _PowerOn();
  _using_partial_mode = false;
}

void GxEPD2_213_B72::_Init_Part()
{
  _InitDisplay();
  _writeCommand(0x2C); //VCOM Voltage
  _writeData(0x26);    // NA ??
  _writeCommand(0x32);
  _writeDataPGM(LUT_DATA_part, sizeof(LUT_DATA_part));
  _PowerOn();
  _using_partial_mode = true;
}

void GxEPD2_213_B72::_Update_Full()
{
  _writeCommand(0x22);
  _writeData(0xc4);
  _writeCommand(0x20);
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_213_B72::_Update_Part()
{
  _writeCommand(0x22);
  _writeData(0x04);
  _writeCommand(0x20);
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}
