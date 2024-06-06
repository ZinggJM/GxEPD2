// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display, available here: https://www.good-display.com/companyfile/32/
// Panel: GDEM1085T51 : https://www.good-display.com/product/503.html
// Controller : JD79686AB :
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_1085_GDEM1085T51.h"

GxEPD2_1085_GDEM1085T51::GxEPD2_1085_GDEM1085T51(int16_t cs, int16_t dc, int16_t rst, int16_t busy, int16_t cs2) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 10000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _cs2 = cs2;
}

void GxEPD2_1085_GDEM1085T51::init(uint32_t serial_diag_bitrate)
{
  if (_cs2 >= 0)
  {
    digitalWrite(_cs2, HIGH); // preset (less glitch for any analyzer)
    pinMode(_cs2, OUTPUT);
    digitalWrite(_cs2, HIGH); // set (needed e.g. for RP2040)
  }
  GxEPD2_EPD::init(serial_diag_bitrate);
}

void GxEPD2_1085_GDEM1085T51::init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration, bool pulldown_rst_mode)
{
  if (_cs2 >= 0)
  {
    digitalWrite(_cs2, HIGH); // preset (less glitch for any analyzer)
    pinMode(_cs2, OUTPUT);
    digitalWrite(_cs2, HIGH); // set (needed e.g. for RP2040)
  }
  GxEPD2_EPD::init(serial_diag_bitrate, initial, reset_duration, pulldown_rst_mode);
}

void GxEPD2_1085_GDEM1085T51::clearScreen(uint8_t value)
{
  // full refresh needed for all cases (previous != screen)
  _writeScreenBuffer(0x10, value); // set previous
  _writeScreenBuffer(0x13, value); // set current
  refresh(false); // full refresh
  _initial_write = false;
}

void GxEPD2_1085_GDEM1085T51::writeScreenBuffer(uint8_t value)
{
  if (_initial_write) return clearScreen(value);
  _writeScreenBuffer(0x13, value); // set current
}

void GxEPD2_1085_GDEM1085T51::writeScreenBufferAgain(uint8_t value)
{
  _writeScreenBuffer(0x10, value); // set previous
  _writeScreenBuffer(0x13, value); // set current
}

void GxEPD2_1085_GDEM1085T51::_writeScreenBuffer(uint8_t command, uint8_t value)
{
  if (!_init_display_done) _InitDisplay();
  _writeCommandToMaster(command);
  _startTransferToMaster();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransferToMaster();
  _writeCommandToSlave(command);
  _startTransferToSlave();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransferToSlave();
}

void GxEPD2_1085_GDEM1085T51::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImage("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _writeImage(0x13, bitmap, x, y, w, h, invert, mirror_y, pgm); // set current
}

//void GxEPD2_1085_GDEM1085T51::writeImageForFullRefresh(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
//{
//  //Serial.print("writeImageForFullRefresh("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
//  _writeImage(0x10, bitmap, x, y, w, h, invert, mirror_y, pgm); // set previous
//  _writeImage(0x13, bitmap, x, y, w, h, invert, mirror_y, pgm); // set current
//}

void GxEPD2_1085_GDEM1085T51::writeImageAgain(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImageAgain("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _writeImage(0x10, bitmap, x, y, w, h, invert, mirror_y, pgm); // set previous
  _writeImage(0x13, bitmap, x, y, w, h, invert, mirror_y, pgm); // set current
}

void GxEPD2_1085_GDEM1085T51::_writeImage(uint8_t command, const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("_writeImage("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
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
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  if (x1 < WIDTH / 2)
  {
    int16_t wm = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    _writeCommandToMaster(0x91); // partial in
    _setPartialRamAreaMaster(x1, y1, wm, h1);
    _writeCommandToMaster(command);
    _startTransferToMaster();
    for (int16_t i = 0; i < h1; i++)
    {
      for (int16_t j = 0; j < wm / 8; j++)
      {
        uint8_t data;
        // use wb, h of bitmap for index!
        int32_t idx = mirror_y ? j + dx / 8 + int32_t((h - 1 - (i + dy))) * wb : j + dx / 8 + int32_t(i + dy) * wb;
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
    _endTransferToMaster();
    _writeCommandToMaster(0x92); // partial out
  }
  if (x1 + w1 > WIDTH / 2)
  {
    int16_t xs = x1 <= WIDTH / 2 ? WIDTH / 2 : x1;
    int16_t ws = x1 <= WIDTH / 2 ? x1 + w1 - WIDTH / 2 : w1;
    _writeCommandToSlave(0x91); // partial in
    _setPartialRamAreaSlave(xs - WIDTH / 2, y1, ws, h1);
    _writeCommandToSlave(command);
    _startTransferToSlave();
    for (int16_t i = 0; i < h1; i++)
    {
      for (int16_t j = 0; j < ws / 8; j++)
      {
        uint8_t data;
        // use wb, h of bitmap for index!
        int32_t idx = mirror_y ? (xs - x1) / 8 + j + int32_t((h - 1 - (i + dy))) * wb : (xs - x1) / 8 + j + int32_t(i + dy) * wb;
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
    _endTransferToSlave();
    _writeCommandToSlave(0x92); // partial out
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_1085_GDEM1085T51::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x13, bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_1085_GDEM1085T51::writeImagePartAgain(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x10, bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_1085_GDEM1085T51::_writeImagePart(uint8_t command, const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("_writeImagePart("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", "); Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
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
  if (x1 < WIDTH / 2)
  {
    int16_t wm = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    _writeCommandToMaster(0x91); // partial in
    _setPartialRamAreaMaster(x1, y1, wm, h1);
    _writeCommandToMaster(command);
    _startTransferToMaster();
    for (int16_t i = 0; i < h1; i++)
    {
      for (int16_t j = 0; j < wm / 8; j++)
      {
        uint8_t data;
        // use wb_bitmap, h_bitmap of bitmap for index!
        int32_t idx = mirror_y ? x_part / 8 + j + dx / 8 + int32_t((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + int32_t(y_part + i + dy) * wb_bitmap;
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
    _endTransferToMaster();
    _writeCommandToMaster(0x92); // partial out
  }
  if (x1 + w1 > WIDTH / 2)
  {
    int16_t xs = x1 <= WIDTH / 2 ? WIDTH / 2 : x1;
    int16_t ws = x1 <= WIDTH / 2 ? x1 + w1 - WIDTH / 2 : w1;
    _writeCommandToSlave(0x91); // partial in
    _setPartialRamAreaSlave(xs - WIDTH / 2, y1, ws, h1);
    _writeCommandToSlave(command);
    _startTransferToSlave();
    for (int16_t i = 0; i < h1; i++)
    {
      for (int16_t j = 0; j < ws / 8; j++)
      {
        uint8_t data;
        // use wb_bitmap, h_bitmap of bitmap for index!
        int32_t idx = mirror_y ? x_part / 8 + (xs - x1) / 8 + j + dx / 8 + int32_t((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + (xs - x1) / 8 + j + dx / 8 + int32_t(y_part + i + dy) * wb_bitmap;
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
    _endTransferToSlave();
    _writeCommandToSlave(0x92); // partial out
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_1085_GDEM1085T51::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    writeImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_1085_GDEM1085T51::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    writeImagePart(black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_1085_GDEM1085T51::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_1085_GDEM1085T51::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
  writeImageAgain(bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_1085_GDEM1085T51::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
  writeImagePartAgain(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_1085_GDEM1085T51::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    drawImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_1085_GDEM1085T51::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    drawImagePart(black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_1085_GDEM1085T51::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    drawImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_1085_GDEM1085T51::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else
  {
    if (_using_partial_mode) _Init_Full();
    _Update_Full();
    _initial_refresh = false; // initial full update done
  }
}

void GxEPD2_1085_GDEM1085T51::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  if (_initial_refresh) return refresh(false); // initial update needs be full update
  // intersection with screen
  int16_t w1 = x < 0 ? w + x : w; // reduce
  int16_t h1 = y < 0 ? h + y : h; // reduce
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  w1 = x1 + w1 < int16_t(WIDTH) ? w1 : int16_t(WIDTH) - x1; // limit
  h1 = y1 + h1 < int16_t(HEIGHT) ? h1 : int16_t(HEIGHT) - y1; // limit
  if ((w1 <= 0) || (h1 <= 0)) return;
  // make x1, w1 multiple of 8
  w1 += x1 % 8;
  if (w1 % 8 > 0) w1 += 8 - w1 % 8;
  x1 -= x1 % 8;
  if (!_using_partial_mode) _Init_Part();
#if 0
  //_writeCommandToBoth(0x91); // partial in
  if (x1 < WIDTH / 2)
  {
    int16_t wm = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    _writeCommandToMaster(0x91); // partial in
    _setPartialRamAreaMaster(x1, y1, wm, h1);
  }
  if (x1 + w1 > WIDTH / 2)
  {
    int16_t xs = x1 <= WIDTH / 2 ? WIDTH / 2 : x1;
    int16_t ws = x1 <= WIDTH / 2 ? x1 + w1 - WIDTH / 2 : w1;
    _writeCommandToSlave(0x91); // partial in
    _setPartialRamAreaSlave(xs - WIDTH / 2, y1, ws, h1);
  }
  _Update_Part();
  _writeCommandToBoth(0x92); // partial out
#else
  _setPartialRamAreaMaster(0, 0, WIDTH / 2, HEIGHT);
  _setPartialRamAreaSlave(0, 0, WIDTH / 2, HEIGHT);
  _Update_Part();
#endif
}

void GxEPD2_1085_GDEM1085T51::powerOff(void)
{
  _PowerOff();
}

void GxEPD2_1085_GDEM1085T51::hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x07); // deep sleep
    _writeData(0xA5);    // check code
    _hibernating = true;
    _init_display_done = false;
  }
}

void GxEPD2_1085_GDEM1085T51::_writeCommandToMaster(uint8_t c)
{
  _writeCommand(c);
}
void GxEPD2_1085_GDEM1085T51::_writeCommandToSlave(uint8_t c)
{
  _pSPIx->beginTransaction(_spi_settings);
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs2 >= 0) digitalWrite(_cs2, LOW);
  _pSPIx->transfer(c);
  if (_cs2 >= 0) digitalWrite(_cs2, HIGH);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
  _pSPIx->endTransaction();
}
void GxEPD2_1085_GDEM1085T51::_writeCommandToBoth(uint8_t c)
{
  _pSPIx->beginTransaction(_spi_settings);
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  if (_cs2 >= 0) digitalWrite(_cs2, LOW);
  _pSPIx->transfer(c);
  if (_cs2 >= 0) digitalWrite(_cs2, HIGH);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
  _pSPIx->endTransaction();
}
void GxEPD2_1085_GDEM1085T51::_writeDataToMaster(uint8_t d)
{
  _writeData(d);
}
void GxEPD2_1085_GDEM1085T51::_writeDataToSlave(uint8_t d)
{
  _pSPIx->beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs2, LOW);
  _pSPIx->transfer(d);
  if (_cs >= 0) digitalWrite(_cs2, HIGH);
  _pSPIx->endTransaction();
}
void GxEPD2_1085_GDEM1085T51::_writeDataToBoth(uint8_t d)
{
  _pSPIx->beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  if (_cs >= 0) digitalWrite(_cs2, LOW);
  _pSPIx->transfer(d);
  if (_cs2 >= 0) digitalWrite(_cs2, HIGH);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _pSPIx->endTransaction();
}
void GxEPD2_1085_GDEM1085T51::_startTransferToMaster()
{
  _startTransfer();
}
void GxEPD2_1085_GDEM1085T51::_startTransferToSlave()
{
  _pSPIx->beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs2, LOW);
}
void GxEPD2_1085_GDEM1085T51::_endTransferToMaster()
{
  _endTransfer();
}
void GxEPD2_1085_GDEM1085T51::_endTransferToSlave()
{
  if (_cs >= 0) digitalWrite(_cs2, HIGH);
  _pSPIx->endTransaction();
}

void GxEPD2_1085_GDEM1085T51::_setPartialRamAreaMaster(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  //Serial.print("_setPartialRamAreaMaster("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  uint16_t xe = (x + w - 1) | 0x0007; // byte boundary inclusive (last byte)
  uint16_t ye = y + h - 1;
  x &= 0xFFF8; // byte boundary
  _writeCommandToMaster(0x90); // partial window
  _writeDataToMaster(x / 256);
  _writeDataToMaster(x % 256);
  _writeDataToMaster(xe / 256);
  _writeDataToMaster(xe % 256);
  _writeDataToMaster(y / 256);
  _writeDataToMaster(y % 256);
  _writeDataToMaster(ye / 256);
  _writeDataToMaster(ye % 256);
  _writeDataToMaster(0x01); // don't see any difference
  //_writeDataToMaster(0x00); // don't see any difference
}

void GxEPD2_1085_GDEM1085T51::_setPartialRamAreaSlave(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  //Serial.print("_setPartialRamAreaSlave("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  uint16_t xe = (x + w - 1) | 0x0007; // byte boundary inclusive (last byte)
  uint16_t ye = y + h - 1;
  x &= 0xFFF8; // byte boundary
  _writeCommandToSlave(0x90); // partial window
  _writeDataToSlave(x / 256);
  _writeDataToSlave(x % 256);
  _writeDataToSlave(xe / 256);
  _writeDataToSlave(xe % 256);
  _writeDataToSlave(y / 256);
  _writeDataToSlave(y % 256);
  _writeDataToSlave(ye / 256);
  _writeDataToSlave(ye % 256);
  _writeDataToSlave(0x01); // don't see any difference
  //_writeData(0x00); // don't see any difference
}

void GxEPD2_1085_GDEM1085T51::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommandToBoth(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_1085_GDEM1085T51::_PowerOff()
{
  if (_power_is_on)
  {
    _writeCommandToBoth(0x02); // power off
    _waitWhileBusy("_PowerOff", power_off_time);
    _power_is_on = false;
  }
}

void GxEPD2_1085_GDEM1085T51::_soft_reset()
{
  if (_hibernating) _reset();
  _writeCommandToBoth(0x00); // Panel setting
  _writeDataToBoth(0x9E);    // RST_N
  delay(2);
  _writeCommandToBoth(0x00); // Panel setting
  _writeDataToBoth(0x9F);    // otp
  _waitWhileBusy("_soft_reset", power_on_time);
  _power_is_on = false;
}

void GxEPD2_1085_GDEM1085T51::_InitDisplay()
{
  if (_hibernating) _reset();
  // from A32-GDEM1085T51-P-20240314
  _writeCommandToBoth(0x4D);
  _writeDataToBoth(0x55);
  _writeCommandToBoth(0xA6);
  _writeDataToBoth(0x38);
  _writeCommandToBoth(0xB4);
  _writeDataToBoth(0x5D);
  _writeCommandToBoth(0xB6);
  _writeDataToBoth(0x80);
  _writeCommandToBoth(0xB7);
  _writeDataToBoth(0x00);
  _writeCommandToBoth(0xF7);
  _writeDataToBoth(0x02);
  _writeCommandToSlave(0xAE);
  _writeDataToSlave(0xA0);
  _writeCommandToBoth(0xE0); // Cascade Setting (CCSET)
  _writeDataToBoth(0x01);    // CCEN (clock for slave)
  //
  _writeCommandToBoth(0x00); // Panel setting
  _writeDataToBoth(0x9F);    // otp
  _writeDataToBoth(0x0D);    // TS_AUTO, TIEG, no NORG, VC_LUTZ
  _writeCommandToBoth(0x06); // Booster Setting
  _writeDataToBoth(0x57);    // 20ms, str 3, period 8
  _writeDataToBoth(0x24);    // 10ms, str 5, period 5
  _writeDataToBoth(0x28);    // 10ms, str 6, period 1
  _writeDataToBoth(0x32);    // ??
  _writeDataToBoth(0x08);    // ??
  _writeDataToBoth(0x48);    // ??
  _writeCommandToBoth(0x50); // VCOM and DATA interval setting
  _writeDataToBoth(0x97);    // LUTBW, 10 hsync
  _writeCommandToBoth(0x60); // TCON setting
  _writeDataToBoth(0x31);    // ??
  _writeCommandToBoth(0x61); // Resolution setting
  _writeDataToBoth(0X02);    // 640
  _writeDataToBoth(0XA8);
  _writeDataToBoth(0X01);    // 480
  _writeDataToBoth(0XE0);
  _writeCommandToBoth(0x62); // Source & gate start setting
  _writeDataToBoth(0x00);
  _writeDataToBoth(0x00);
  _writeDataToBoth(0x00);
  _writeDataToBoth(0x00);
  //_writeCommandToBoth(0xE8);
  //_writeDataToBoth(0x01);
  _init_display_done = true;
}

void GxEPD2_1085_GDEM1085T51::_Init_Full()
{
  //Serial.println("_Init_Full");
  if (_using_partial_mode) _soft_reset();
  _InitDisplay();
  _writeCommandToBoth(0xE8);
  _writeDataToBoth(0x01);
  _PowerOn();
  _using_partial_mode = false;
}

void GxEPD2_1085_GDEM1085T51::_Init_Part()
{
  //Serial.println("_Init_Part");
  if (!_using_partial_mode) _soft_reset();
  _InitDisplay();
  _writeCommandToBoth(0xE0); // Cascade Setting (CCSET)
  _writeDataToBoth(0x03);    // TSFIX, CCEN (clock for slave)
  _writeCommandToBoth(0xE5); // Force Temperature
  _writeDataToBoth(0x64);    // 100, differential update
  _PowerOn();
  _using_partial_mode = true;
}

void GxEPD2_1085_GDEM1085T51::_Update_Full()
{
  _PowerOn();
  _writeCommandToBoth(0xE0); // Cascade Setting (CCSET)
  _writeDataToBoth(0x01);    // no TSFIX, CCEN (clock for slave)
  _writeCommandToBoth(0xE8);
  _writeDataToBoth(0x01);
  _writeCommandToBoth(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_1085_GDEM1085T51::_Update_Part()
{
  _PowerOn();
  _writeCommandToBoth(0x12); //display refresh
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}
