// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: https://www.good-display.com/comp/xcompanyFile/downloadNew.do?appId=24&fid=1370&id=1147
// Panel: GDEY0579Z93 : https://www.good-display.com/product/441.html
// Controller : SSD1683 : https://v4.cecdn.yun300.cn/100001_1909185148/SSD1683.PDF
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_579c_GDEY0579Z93.h"

GxEPD2_579c_GDEY0579Z93::GxEPD2_579c_GDEY0579Z93(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, HIGH, 25000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}

void GxEPD2_579c_GDEY0579Z93::clearScreen(uint8_t value)
{
  clearScreen(value, 0x00);
}

void GxEPD2_579c_GDEY0579Z93::clearScreen(uint8_t black_value, uint8_t color_value)
{
  writeScreenBuffer(black_value, color_value);
  refresh(false);
}

void GxEPD2_579c_GDEY0579Z93::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0x00);
}

void GxEPD2_579c_GDEY0579Z93::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  if (!_init_display_done) _InitDisplay();
  _writeScreenBuffer(0x24, black_value); // set black/white
  _writeScreenBuffer(0x26, color_value); // set red/white
  _initial_write = false; // initial full screen buffer clean done
}

void GxEPD2_579c_GDEY0579Z93::_writeScreenBuffer(uint8_t command, uint8_t value)
{
  if (!_init_display_done) _InitDisplay();
  _setPartialRamAreaMaster(0, 0, WIDTH / 2, HEIGHT);
  _writeCommand(command);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransfer();
  _setPartialRamAreaMaster(0, HEIGHT / 2, WIDTH / 2, HEIGHT);
  _writeCommand(command);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransfer();
  _setPartialRamAreaSlave(0, 0, WIDTH / 2, HEIGHT / 2);
  _writeCommand(command | 0x80);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransfer();
  _setPartialRamAreaSlave(0, HEIGHT / 2, WIDTH / 2, HEIGHT);
  _writeCommand(command | 0x80);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransfer();
}

void GxEPD2_579c_GDEY0579Z93::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeScreenBuffer(0x26, 0x00); // set red/white
  _writeImage(0x24, bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_579c_GDEY0579Z93::_writeImage(uint8_t command, const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImage("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  int16_t wbb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x >= 0 ? x % 8 : -x % 8; // byte boundary
  w = wbb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx1 = x1 - x; // offset into bitmap
  int16_t dy1 = y1 - y; // offset into bitmap
  w1 -= dx1;
  h1 -= dy1;
  if ((w1 <= 0) || (h1 <= 0)) return;
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  _writeFromImage(command, bitmap, x1, y1, w1, h1, wbb, h, dx1, dy1, invert, mirror_y, pgm);
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_579c_GDEY0579Z93::_writeFromImage(uint8_t command, const uint8_t bitmap[], int16_t x1, int16_t y1, int16_t w1, int16_t h1,
    int16_t wbb, int16_t hb, int16_t dx, int16_t dy, bool invert, bool mirror_y, bool pgm)
{
  if ((x1 < WIDTH / 2) && (y1 < HEIGHT / 2) && true) // upper left, near connector
  { // memory is on slave controller
    int16_t ws = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    int16_t hs = y1 + h1 <= HEIGHT / 2 ? h1 : HEIGHT / 2 - y1;
    //Serial.print("upper left ws="); Serial.print(ws); Serial.print(" hs="); Serial.print(hs); Serial.print(" dx="); Serial.print(dx); Serial.print(" dy="); Serial.println(dy);
    _setPartialRamAreaSlave(x1, y1, ws, hs, 0x03);
    _writeCommand(command | 0x80);
    _writeDataFromImage(bitmap, ws, hs, wbb, hb, dx, dy, invert, mirror_y, pgm);
  }

  if (((x1 + w1) > WIDTH / 2) && (y1 < HEIGHT / 2) && true) // upper right, opposite to connector
  { // memory is on master controller
    int16_t wm = x1 >= WIDTH / 2 ? w1 : (x1 + w1) - WIDTH / 2;
    int16_t hm = y1 + h1 <= HEIGHT / 2 ? h1 : HEIGHT / 2 - y1;
    int16_t xm = x1 < WIDTH / 2 ? WIDTH / 2 - wm : WIDTH - x1 - wm;
    int16_t ym = y1 < HEIGHT / 2 ? HEIGHT / 2 - hm : HEIGHT - y1 - hm;
    //Serial.print("upper right wm="); Serial.print(wm); Serial.print(" hm="); Serial.print(hm); Serial.print(" dx="); Serial.print(dx); Serial.print(" dy="); Serial.println(dy);
    _setPartialRamAreaMaster(xm, y1, wm, hm, 0x02);
    _writeCommand(command);
    _writeDataFromImage(bitmap, wm, hm, wbb, hb, dx + w1 - wm, dy, invert, mirror_y, pgm);
  }

  if ((x1 < WIDTH / 2) && ((y1 + h1) > HEIGHT / 2) && true) // lower left, near connector
  { // memory is on slave controller
    int16_t ws = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    int16_t hs = y1 >= HEIGHT / 2 ? h1 : (y1 + h1) - HEIGHT / 2;
    int16_t ys = y1 >= HEIGHT / 2 ? y1 : HEIGHT / 2;
    //Serial.print("lower left ws="); Serial.print(ws); Serial.print(" hs="); Serial.print(hs); Serial.print(" dx="); Serial.print(dx); Serial.print(" dy="); Serial.println(dy);
    _setPartialRamAreaSlave(x1, ys, ws, hs, 0x03);
    _writeCommand(command | 0x80);
    _writeDataFromImage(bitmap, ws, hs, wbb, hb, dx, dy + h1 - hs, invert, mirror_y, pgm);
  }

  if (((x1 + w1) > WIDTH / 2) && ((y1 + h1) > HEIGHT / 2) && true) // lower right, opposite to connector
  { // memory is on master controller
    int16_t wm = x1 >= WIDTH / 2 ? w1 : (x1 + w1) - WIDTH / 2;
    int16_t hm = y1 >= HEIGHT / 2 ? h1 : (y1 + h1) - HEIGHT / 2;
    int16_t xm = x1 < WIDTH / 2 ? WIDTH / 2 - wm : WIDTH - x1 - wm;
    int16_t ym = y1 >= HEIGHT / 2 ? y1 : HEIGHT / 2;
    //Serial.print("lower right wm="); Serial.print(wm); Serial.print(" hm="); Serial.print(hm); Serial.print(" dx="); Serial.print(dx); Serial.print(" dy="); Serial.println(dy);
    _setPartialRamAreaMaster(xm, ym, wm, hm, 0x02);
    _writeCommand(command);
    _writeDataFromImage(bitmap, wm, hm, wbb, hb, dx + w1 - wm, dy + h1 - hm, invert, mirror_y, pgm);
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_579c_GDEY0579Z93::_writeDataFromImage(const uint8_t bitmap[], int16_t w, int16_t h, int16_t wbb, int16_t hb, int16_t dx, int16_t dy, bool invert, bool mirror_y, bool pgm)
{
    _startTransfer();
    for (int16_t i = 0; i < h; i++)
    {
      for (int16_t j = 0; j < w; j += 8)
      {
        uint8_t data;
        // use wbb, h of bitmap for index!
        int16_t idx = mirror_y ? ((j + dx) / 8 + ((hb - 1 - (i + dy))) * wbb) : ((j + dx) / 8 + (i + dy) * wbb);
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
        _transfer(data);
      }
    }
    _endTransfer();
}

void GxEPD2_579c_GDEY0579Z93::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x24, bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_579c_GDEY0579Z93::_writeImagePart(uint8_t command, const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImagePart("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", ");
  //Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
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
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  _writeFromImage(command, bitmap, x1, y1, w1, h1, wb_bitmap, h_bitmap, dx + x_part, dy + y_part, invert, mirror_y, pgm);
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_579c_GDEY0579Z93::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black) _writeImage(0x24, black, x, y, w, h, invert, mirror_y, pgm);
  if (color) _writeImage(0x26, color, x, y, w, h, !invert, mirror_y, pgm);
}

void GxEPD2_579c_GDEY0579Z93::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black) _writeImagePart(0x24, black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  if (color) _writeImagePart(0x26, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, !invert, mirror_y, pgm);
}

void GxEPD2_579c_GDEY0579Z93::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_579c_GDEY0579Z93::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_579c_GDEY0579Z93::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_579c_GDEY0579Z93::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_579c_GDEY0579Z93::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_579c_GDEY0579Z93::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_579c_GDEY0579Z93::refresh(bool partial_update_mode)
{
  _Update_Full(); // always uses full window refresh
}

void GxEPD2_579c_GDEY0579Z93::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  _Update_Full(); // always uses full window refresh
}

void GxEPD2_579c_GDEY0579Z93::powerOff()
{
  _PowerOff();
}

void GxEPD2_579c_GDEY0579Z93::hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x10); // deep sleep
    _writeData(0x11);    // deep sleep
    _hibernating = true;
    _init_display_done = false;
  }
}

void GxEPD2_579c_GDEY0579Z93::_setPartialRamAreaMaster(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t mode)
{
  //Serial.print("_setPartialRamAreaMaster("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _setPartialRamArea(x, y, w, h, mode, 0x00);
}

void GxEPD2_579c_GDEY0579Z93::_setPartialRamAreaSlave(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t mode)
{
  //Serial.print("_setPartialRamAreaSlave("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _setPartialRamArea(x, y, w, h, mode, 0x80);
}

void GxEPD2_579c_GDEY0579Z93::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t mode, uint8_t target)
{
  _writeCommand(0x11 | target); // set ram entry mode
  _writeData(mode);
  switch (mode)
  {
    case 0x00: // x decrement, y decrement
      _writeCommand(0x44 | target);
      _writeData((x + w - 1) / 8);
      _writeData(x / 8);
      _writeCommand(0x45 | target);
      _writeData((y + h - 1) % 256);
      _writeData((y + h - 1) / 256);
      _writeData(y % 256);
      _writeData(y / 256);
      _writeCommand(0x4e | target);
      _writeData((x + w - 1) / 8);
      _writeCommand(0x4f | target);
      _writeData((y + h - 1) % 256);
      _writeData((y + h - 1) / 256);
      break;
    case 0x01: // x increment, y decrement
      _writeCommand(0x44 | target);
      _writeData(x / 8);
      _writeData((x + w - 1) / 8);
      _writeCommand(0x45 | target);
      _writeData((y + h - 1) % 256);
      _writeData((y + h - 1) / 256);
      _writeData(y % 256);
      _writeData(y / 256);
      _writeCommand(0x4e | target);
      _writeData(x / 8);
      _writeCommand(0x4f | target);
      _writeData((y + h - 1) % 256);
      _writeData((y + h - 1) / 256);
      break;
    case 0x02: // x decrement, y increment
      _writeCommand(0x44 | target);
      _writeData((x + w - 1) / 8);
      _writeData(x / 8);
      _writeCommand(0x45 | target);
      _writeData(y % 256);
      _writeData(y / 256);
      _writeData((y + h - 1) % 256);
      _writeData((y + h - 1) / 256);
      _writeCommand(0x4e | target);
      _writeData((x + w - 1) / 8);
      _writeCommand(0x4f | target);
      _writeData(y % 256);
      _writeData(y / 256);
      break;
    case 0x03: // x increment, y increment
      _writeCommand(0x44 | target);
      _writeData(x / 8);
      _writeData((x + w - 1) / 8);
      _writeCommand(0x45 | target);
      _writeData(y % 256);
      _writeData(y / 256);
      _writeData((y + h - 1) % 256);
      _writeData((y + h - 1) / 256);
      _writeCommand(0x4e | target);
      _writeData(x / 8);
      _writeCommand(0x4f | target);
      _writeData(y % 256);
      _writeData(y / 256);
      break;
  }
  delay(2);
}

void GxEPD2_579c_GDEY0579Z93::_PowerOn()
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

void GxEPD2_579c_GDEY0579Z93::_PowerOff()
{
  if (_power_is_on)
  {
    _writeCommand(0x22);
    _writeData(0xc3);
    _writeCommand(0x20);
    _waitWhileBusy("_PowerOff", power_off_time);
  }
  _power_is_on = false;
}

void GxEPD2_579c_GDEY0579Z93::_InitDisplay()
{
  if (_hibernating) _reset();
  _writeCommand(0x12);  //SWRESET
  delay(10); // 10ms according to specs, 4ms measured
  _writeCommand(0x18); // Temperature Sensor Control
  _writeData(0x80);    // Internal temperature sensor
  _writeCommand(0x22); // Display Update Control 2
  _writeData(0xB1);    // Enable clock signal, Load temperature value, Load LUT (3-color mode), Disable clock signal
  _writeCommand(0x20); // Master Activation
  delay(10); // 4ms measured
  _writeCommand(0x1A); // Write to temperature register
  _writeData(0x64);
  _writeData(0x00);
  _writeCommand(0x22); // Display Update Control 2
  _writeData(0xB1);    // Enable clock signal, Load temperature value, Load LUT (3-color mode), Disable clock signal
  _writeCommand(0x20); // Master Activation
  delay(10); // 4ms measured
  _init_display_done = true;
}

void GxEPD2_579c_GDEY0579Z93::_Update_Full()
{
  _writeCommand(0x22); // Display Update Sequence Options
  _writeData(0xF7);    //
  _writeCommand(0x20); // Master Activation
  _waitWhileBusy("_Update_Full", full_refresh_time);
  _power_is_on = false;
}
