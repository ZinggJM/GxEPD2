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

#include "GxEPD2_576_GDEH0576T81.h"

GxEPD2_576_GDEH0576T81::GxEPD2_576_GDEH0576T81(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 25000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _using_differential_refresh = false;
}

void GxEPD2_576_GDEH0576T81::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_576_GDEH0576T81::clearScreen(uint8_t black_value, uint8_t color_value)
{
  writeScreenBuffer(black_value, color_value);
  refresh();
}

void GxEPD2_576_GDEH0576T81::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_576_GDEH0576T81::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  if (!_init_display_done) _InitDisplay();
  _use_differential_refresh(false);
  _setPartialRamArea(0, 0, WIDTH, HEIGHT, false);
  _writeCommand(0x10);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 4; i++)
  {
    _transfer(0xFF == black_value ? 0xFF : 0x00);
  }
  _endTransfer();
  _initial_write = false; // initial full screen buffer clean done
}

void GxEPD2_576_GDEH0576T81::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImage("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  _use_differential_refresh(false);
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
      uint8_t data = 0xFF;
      // use wb, h of bitmap for index!
      uint32_t idx = mirror_y ? j + dx / 8 + uint32_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint32_t(i + dy) * wb;
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
      for (int16_t k = 0; k < 2; k++)
      {
        uint8_t data2 = (data & 0x80 ? 0xC0 : 0x00) | (data & 0x40 ? 0x30 : 0x00) |
                        (data & 0x20 ? 0x0C : 0x00) | (data & 0x10 ? 0x03 : 0x00);
        data <<= 4;
        _transfer(data2);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_576_GDEH0576T81::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!black) return;
  writeImage(black, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_576_GDEH0576T81::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImagePart("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", ");
  //Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  _use_differential_refresh(false);
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
      for (int16_t k = 0; k < 2; k++)
      {
        uint8_t data2 = (data & 0x80 ? 0x40 : 0x00) | (data & 0x40 ? 0x10 : 0x00) |
                        (data & 0x20 ? 0x04 : 0x00) | (data & 0x10 ? 0x01 : 0x00);
        data <<= 4;
        _transfer(data2);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_576_GDEH0576T81::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!black) return;
  writeImagePart(black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

// for this controller used to transfer bitmaps of previous and current content for differential refresh
void GxEPD2_576_GDEH0576T81::writeNative(const uint8_t* previous, const uint8_t* current, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!previous || !current) return;
  //Serial.print("writeNative("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  _use_differential_refresh(true);
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
      uint8_t new_data = 0xFF, old_data = 0xFF;
      // use wb, h of bitmap for index!
      uint32_t idx = mirror_y ? j + dx / 8 + uint32_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint32_t(i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        new_data = pgm_read_byte(&current[idx]);
        old_data = pgm_read_byte(&previous[idx]);
#else
        new_data = current[idx];
        old_data = previous[idx];
#endif
      }
      else
      {
        new_data = current[idx];
        old_data = previous[idx];
      }
      if (invert)
      {
        new_data = ~new_data;
        old_data = ~old_data;
      }
      for (int16_t k = 0; k < 2; k++)
      {
        uint8_t out_data = (new_data & 0x80 ? 0x40 : 0x00) | (new_data & 0x40 ? 0x10 : 0x00) |
                           (new_data & 0x20 ? 0x04 : 0x00) | (new_data & 0x10 ? 0x01 : 0x00) |
                           (old_data & 0x80 ? 0x80 : 0x00) | (old_data & 0x40 ? 0x20 : 0x00) |
                           (old_data & 0x20 ? 0x08 : 0x00) | (old_data & 0x10 ? 0x02 : 0x00);
        new_data <<= 4;
        old_data <<= 4;
        _transfer(out_data);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

// for this controller used to transfer bitmaps of previous and current content for differential refresh
void GxEPD2_576_GDEH0576T81::writeNativePart(const uint8_t* previous, const uint8_t* current, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!previous || !current) return;
  //Serial.print("writeNativePart("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", ");
  //Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  _use_differential_refresh(true);
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wb_bitmap = (w_bitmap + 7) / 8; // width bytes, bitmaps are padded
  x_part -= x_part % 8; // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w; // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h; // limit
  w += x % 8; // adjust for byte boundary of x
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
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x10);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t new_data = 0xFF, old_data = 0xFF;
      // use wb_bitmap, h_bitmap of bitmap for index!
      uint32_t idx = mirror_y ? x_part / 8 + j + dx / 8 + uint32_t((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + uint32_t(y_part + i + dy) * wb_bitmap;
      uint32_t ido = idx;
      if ((x_part == 0) && (y_part == 0) && (w_bitmap == w) && (h_bitmap == h)) // the parameters used for partial window
      {
        // for partial window : current is transposed to 0,0 : use previous untransposed (as is the target)
        ido = j + x1 / 8 + uint32_t(i + y1) * (WIDTH / 8);
      }
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        new_data = pgm_read_byte(&current[idx]);
        old_data = pgm_read_byte(&previous[ido]);
#else
        new_data = current[idx];
        old_data = previous[ido];
#endif
      }
      else
      {
        new_data = current[idx];
        old_data = previous[ido];
      }
      if (invert)
      {
        new_data = ~new_data;
        old_data = ~old_data;
      }
      for (int16_t k = 0; k < 2; k++)
      {
        uint8_t out_data = (new_data & 0x80 ? 0x40 : 0x00) | (new_data & 0x40 ? 0x10 : 0x00) |
                           (new_data & 0x20 ? 0x04 : 0x00) | (new_data & 0x10 ? 0x01 : 0x00) |
                           (old_data & 0x80 ? 0x80 : 0x00) | (old_data & 0x40 ? 0x20 : 0x00) |
                           (old_data & 0x20 ? 0x08 : 0x00) | (old_data & 0x10 ? 0x02 : 0x00);
        new_data <<= 4;
        old_data <<= 4;
        _transfer(out_data);
      }
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_576_GDEH0576T81::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_576_GDEH0576T81::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_576_GDEH0576T81::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_576_GDEH0576T81::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_576_GDEH0576T81::drawNative(const uint8_t* previous, const uint8_t* current, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(previous, current, x, y, w, h, invert, mirror_y, pgm);
  _refresh();
}

void GxEPD2_576_GDEH0576T81::refresh(bool partial_update_mode)
{
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x50); // CDI
  _writeData(partial_update_mode ? 0x97 : 0x77);    // border floating or white
  _refresh();
}

void GxEPD2_576_GDEH0576T81::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  // intersection with screen
  int16_t w1 = x < 0 ? w + x : w; // reduce
  int16_t h1 = y < 0 ? h + y : h; // reduce
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  w1 = x1 + w1 < int16_t(WIDTH) ? w1 : int16_t(WIDTH) - x1; // limit
  h1 = y1 + h1 < int16_t(HEIGHT) ? h1 : int16_t(HEIGHT) - y1; // limit
  if ((w1 <= 0) || (h1 <= 0)) return;
  // make x1, w1 multiple of 4
  w1 += x1 % 4;
  if (w1 % 4 > 0) w1 += 4 - w1 % 4;
  x1 -= x1 % 4;
  bool fullscreen = ((x1 == 0) && (y1 == 0) && (w1 == WIDTH) && (h1 == HEIGHT));
  _setPartialRamArea(x1, y1, w1, h1, !fullscreen);
  _writeCommand(0x50); // CDI
  _writeData(fullscreen ? 0x77 : 0x97);    // border white or floating
  _refresh();
}

void GxEPD2_576_GDEH0576T81::powerOff()
{
  _PowerOff();
}

void GxEPD2_576_GDEH0576T81::hibernate()
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

void GxEPD2_576_GDEH0576T81::_refresh()
{
  _PowerOn();
  _writeCommand(0x12); // Display Refresh
  _writeData(0x00);
  delay(1);
  _waitWhileBusy("_refresh", full_refresh_time);
}

void GxEPD2_576_GDEH0576T81::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial_mode)
{
  //Serial.print("_setPartialRamArea("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _writeCommand(0x83);
  _writeData(x / 256);
  _writeData(x % 256);
  _writeData((x + w - 1) / 256);
  _writeData((x + w - 1) % 256);
  _writeData(y / 256);
  _writeData(y % 256);
  _writeData((y + h - 1) / 256);
  _writeData((y + h - 1) % 256);
  _writeData(partial_mode ? 0x01 : 0x00);
}

void GxEPD2_576_GDEH0576T81::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_576_GDEH0576T81::_PowerOff()
{
  if (_power_is_on)
  {
    _writeCommand(0x02);
    _writeData(0x00);
    _waitWhileBusy("_PowerOff", power_off_time);
  }
  _power_is_on = false;
}

void GxEPD2_576_GDEH0576T81::_InitDisplay()
{
  if ((_rst >= 0) && (_hibernating || _initial_write))
  {
    delay(100); // At least 20ms delay, 100ms from example
    digitalWrite(_rst, LOW); // Module reset
    delay(10); // At least 10ms delay
    digitalWrite(_rst, HIGH);
    //_waitWhileBusy("_InitDisplay reset", power_on_time); // 5000us measured
    delay(10); // At least 10ms delay
    _hibernating = false;
    _power_is_on = false;
  }
  _writeCommand(0x00); // PSR
  //_writeData(0x27);
  _writeData(0x2F);
  _writeData(0x0E);
  _writeCommand(0x06); // BTST
  _writeData(0x0F);
  _writeData(0x8B);
  _writeData(0x9C);
  _writeData(0xC1);
  _writeCommand(0xE7); // PST
  _writeData(0xC1);
  _writeCommand(0x30); // PLL
  _writeData(0x08);
  _writeCommand(0x50); // CDI
  _writeData(0x77);    // border white
  _writeCommand(0x61); // TRES
  _writeData(WIDTH / 256);
  _writeData(WIDTH % 256);
  _writeData(HEIGHT / 256);
  _writeData(HEIGHT % 256);
  _writeCommand(0x62); // HTOTAL
  _writeData(0x98);
  _writeData(0x98);
  _writeData(0x98);
  _writeData(0x75);
  _writeData(0xCA);
  _writeData(0xB2);
  _writeData(0x98);
  _writeData(0x7E);
  _writeCommand(0x65); // GSST
  _writeData(0x00);
  _writeData(0x00);
  _writeData(0x00);
  _writeData(0x00);
  _writeCommand(0xE9); // PST
  _writeData(0x01);
  _lut_temperature = _get_lut_temperature();
  //Serial.print("temperature for LUT selection: "); Serial.println(_lut_temperature);
  _writeCommand(0xE0); // PWS
  _writeData(0x02);
  _writeCommand(0xE6);
  _writeData(_lut_temperature);
  _writeCommand(0xA5);
  delay(2);
  _PowerOn();
  _init_display_done = true;
}

void GxEPD2_576_GDEH0576T81::_use_differential_refresh(bool yes)
{
  if (yes && !_using_differential_refresh)
  {
    _writeCommand(0x50); // CDI
    _writeData(0x97);    // border floating
    _writeCommand(0xE0); // PWS
    _writeData(0x00);
    _writeCommand(0xA5);
    delay(2);
    _using_differential_refresh = true;
  }
  else if (!yes && _using_differential_refresh)
  {
    _writeCommand(0x50); // CDI
    _writeData(0x77);    // border white
    _writeCommand(0xE0); // PWS
    _writeData(0x02);
    _writeCommand(0xE6);
    _writeData(_lut_temperature);
    _writeCommand(0xA5);
    delay(2);
    _using_differential_refresh = false;
  }
}

uint8_t GxEPD2_576_GDEH0576T81::_get_lut_temperature()
{
  _writeCommand(0x40); // read temperature
  delay(5);
  uint8_t temp = _readData();
  //Serial.print("temperature measured for LUT selection: "); Serial.println(temp);
  if (temp == 0) return 241; // didn't get one, use value for 20..30
  if (temp <= 5) return 232;
  if (temp <= 10) return 235;
  if (temp <= 20) return 238;
  if (temp <= 30) return 241;
  if (temp <= 127) return 244;
  return 242;
}
