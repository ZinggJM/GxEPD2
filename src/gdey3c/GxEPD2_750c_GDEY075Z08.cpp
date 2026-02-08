// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: https://www.good-display.com/companyfile/1391.html
// Panel: GDEY075Z08 : https://www.good-display.com/product/394.html
// Controller: UC8179 : https://v4.cecdn.yun300.cn/100001_1909185148/UC8179.pdf
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_750c_GDEY075Z08.h"

GxEPD2_750c_GDEY075Z08::GxEPD2_750c_GDEY075Z08(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 30000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _use_fast_full_update = useFastFullUpdate;
}

void GxEPD2_750c_GDEY075Z08::selectFastFullUpdate(bool ff)
{
  _use_fast_full_update = ff;
}

void GxEPD2_750c_GDEY075Z08::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_750c_GDEY075Z08::clearScreen(uint8_t black_value, uint8_t color_value)
{
  writeScreenBuffer(black_value, color_value);
  refresh(false);
}

void GxEPD2_750c_GDEY075Z08::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_750c_GDEY075Z08::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  if (!_init_display_done) _InitDisplay();
  _writeScreenBuffer(0x10, black_value); // set black/white
  _writeScreenBuffer(0x13, color_value); // set red/white
  _initial_write = false; // initial full screen buffer clean done
}

void GxEPD2_750c_GDEY075Z08::_writeScreenBuffer(uint8_t command, uint8_t value)
{
  //_writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(command);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransfer();
  //_writeCommand(0x92); // partial out
}

void GxEPD2_750c_GDEY075Z08::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeScreenBuffer(0x13, 0xFF); // set red/white
  _writeImage(0x10, bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::_writeImage(uint8_t command, const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  uint16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
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
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(command);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      // use wb, h of bitmap for index!
      uint16_t idx = mirror_y ? j + dx / 8 + uint16_t((h - 1 - (i + dy))) * wb : j + dx / 8 + uint16_t(i + dy) * wb;
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
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_750c_GDEY075Z08::writeImageToPrevious(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImage(0x13, bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::writeImageToCurrent(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImage(0x10, bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x10, bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::_writeImagePart(uint8_t command, const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  uint16_t wb_bitmap = (w_bitmap + 7) / 8; // width bytes, bitmaps are padded
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
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(command);
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
      _transfer(data);
    }
  }
  _endTransfer();
  _writeCommand(0x92); // partial out
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_750c_GDEY075Z08::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black) _writeImage(0x10, black, x, y, w, h, invert, mirror_y, pgm);
  if (color) _writeImage(0x13, color, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black) _writeImagePart(0x10, black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  if (color) _writeImagePart(0x13, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::writeImagePartToPrevious(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x13, bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::writeImagePartToCurrent(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x10, bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_750c_GDEY075Z08::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_750c_GDEY075Z08::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_GDEY075Z08::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_GDEY075Z08::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_GDEY075Z08::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_GDEY075Z08::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_750c_GDEY075Z08::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else _Update_Full();
}

void GxEPD2_750c_GDEY075Z08::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
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
  if (usePartialUpdate) _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
  if (usePartialUpdate) _writeCommand(0x92); // partial out
}

void GxEPD2_750c_GDEY075Z08::powerOff()
{
  _PowerOff();
}

void GxEPD2_750c_GDEY075Z08::hibernate()
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

void GxEPD2_750c_GDEY075Z08::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = (x + w - 1) | 0x0007; // byte boundary inclusive (last byte)
  uint16_t ye = y + h - 1;
  x &= 0xFFF8; // byte boundary
  xe |= 0x0007; // byte boundary
  _writeCommand(0x90); // partial window
  _writeData(x / 256);
  _writeData(x % 256);
  _writeData(xe / 256);
  _writeData(xe % 256);
  _writeData(y / 256);
  _writeData(y % 256);
  _writeData(ye / 256);
  _writeData(ye % 256);
  _writeData(0x00);
}

void GxEPD2_750c_GDEY075Z08::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_750c_GDEY075Z08::_PowerOff()
{
  if (_power_is_on)
  {
    _writeCommand(0x02); // power off
    _waitWhileBusy("_PowerOff", power_off_time);
    _power_is_on = false;
  }
}

void GxEPD2_750c_GDEY075Z08::_InitDisplay()
{
  if (_hibernating) _reset();
  _writeCommand(0x01); // POWER SETTING
  _writeData (0x07);
  _writeData (0x07);   // VGH=20V,VGL=-20V
  _writeData (0x3f);   // VDH=15V
  _writeData (0x3f);   // VDL=-15V
  _writeCommand(0x06); // Booster Soft Start
  _writeData (0x17);
  _writeData (0x17);
  _writeData (0x28);
  _writeData (0x17);
  _writeCommand(0x00); // PANEL SETTING
  _writeData(0x0f);    // KW: 3f, KWR: 2f, BWROTP: 0f, BWOTP: 1f
  _writeCommand(0x61); //resolution setting
  _writeData (WIDTH / 256); //source 800
  _writeData (WIDTH % 256);
  _writeData (HEIGHT / 256); //gate 480
  _writeData (HEIGHT % 256);
  _writeCommand(0x15);
  _writeData(0x00);
  _writeCommand(0x50); // VCOM AND DATA INTERVAL SETTING
  _writeData(0x13);    // 0xFF white for black, 0xFF white for red
  _writeData(0x07);
  _writeCommand(0x60); // TCON SETTING
  _writeData(0x22);
  _init_display_done = true;
}

void GxEPD2_750c_GDEY075Z08::_Update_Full()
{
  _writeCommand(0x00); // PANEL SETTING
  _writeData(0x0f);    // KW: 3f, KWR: 2F, BWROTP: 0f, BWOTP: 1f
  _writeCommand(0x50); // VCOM AND DATA INTERVAL SETTING
  _writeData(0x13);    // 0xFF white for black, 0xFF white for red
  _writeData(0x07);
  if (_use_fast_full_update)
  {
    _writeCommand(0xE0); // Cascade Setting (CCSET)
    _writeData(0x02);    // TSFIX
    _writeCommand(0xE5); // Force Temperature (TSSET)
    _writeData(0x5A);    // 90
  }
  else
  {
    _writeCommand(0xE0); // Cascade Setting (CCSET)
    _writeData(0x00);    // no TSFIX, Temperature value is defined by internal temperature sensor
    _writeCommand(0x41); // TSE, Enable Temperature Sensor
    _writeData(0x00);    // TSE, Internal temperature sensor switch
  }
  _PowerOn();
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
  _PowerOff();
}

void GxEPD2_750c_GDEY075Z08::_Update_Part()
{
  _writeCommand(0x00); // PANEL SETTING
  _writeData(0x0f);    // KW: 3f, KWR: 2F, BWROTP: 0f, BWOTP: 1f
  _writeCommand(0x50); // VCOM AND DATA INTERVAL SETTING
  _writeData(0x93);    // 0xFF white for black, 0xFF white for red, floating border
  _writeData(0x07);
  if (_use_fast_full_update)
  {
    _writeCommand(0xE0); // Cascade Setting (CCSET)
    _writeData(0x02);    // TSFIX
    _writeCommand(0xE5); // Force Temperature (TSSET)
    _writeData(0x5A);    // 90
  }
  else
  {
    _writeCommand(0xE0); // Cascade Setting (CCSET)
    _writeData(0x00);    // no TSFIX, Temperature value is defined by internal temperature sensor
    _writeCommand(0x41); // TSE, Enable Temperature Sensor
    _writeData(0x00);    // TSE, Internal temperature sensor switch
  }
  _PowerOn();
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part", partial_refresh_time);
  _PowerOff();
}

const unsigned char GxEPD2_750c_GDEY075Z08::lut_20_LUTC_partial[] PROGMEM =
{
  // original values copied from OTP
  0x0, 0x3, 0x5, 0x0, 0x0, 0x2,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x3,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const unsigned char GxEPD2_750c_GDEY075Z08::lut_21_LUTWW_partial[] PROGMEM =
{ // 10 w
  // original values copied from OTP, faulty
  //  0x20, 0x3, 0x5, 0x0, 0x0, 0x2,
  //  0x20, 0x5, 0x5, 0x0, 0x0, 0x1,
  //  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  //  0x0, 0x5, 0x5, 0x0, 0x0, 0x3,
  //  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  //  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  //  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  // values copied from OTP, modified
  0x0, 0x3, 0x5, 0x0, 0x0, 0x2,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x3,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const unsigned char GxEPD2_750c_GDEY075Z08::lut_22_LUTKW_partial[] PROGMEM =
{ // 10 w
  // original values copied from OTP
  0x20, 0x3, 0x5, 0x0, 0x0, 0x2,
  0x20, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x20, 0x5, 0x5, 0x0, 0x0, 0x3,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const unsigned char GxEPD2_750c_GDEY075Z08::lut_23_LUTWK_partial[] PROGMEM =
{ // 01 b
  // original values copied from OTP
  0x40, 0x3, 0x5, 0x0, 0x0, 0x2,
  0x40, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x40, 0x5, 0x5, 0x0, 0x0, 0x3,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const unsigned char GxEPD2_750c_GDEY075Z08::lut_24_LUTKK_partial[] PROGMEM =
{ // 01 b
  // original values copied from OTP, faulty
  //  0x40, 0x3, 0x5, 0x0, 0x0, 0x2,
  //  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  //  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  //  0x0, 0x5, 0x5, 0x0, 0x0, 0x3,
  //  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  //  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  //  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  // values copied from OTP, modified
  0x0, 0x3, 0x5, 0x0, 0x0, 0x2,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x1,
  0x0, 0x5, 0x5, 0x0, 0x0, 0x3,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const unsigned char GxEPD2_750c_GDEY075Z08::lut_25_LUTBD_partial[] PROGMEM =
{
  //0x00, T_BW, T_WB, 0, 0, 1,
};

void GxEPD2_750c_GDEY075Z08::refresh_bw(int16_t x, int16_t y, int16_t w, int16_t h)
{
  x -= x % 8; // byte boundary
  w -= w % 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  w1 -= x1 - x;
  h1 -= y1 - y;
  _writeCommand(0x00); // PANEL SETTING
  _writeData(0x1f);    // KW: 3f, KWR: 2F, BWROTP: 0f, BWOTP: 1f
  if (!useFastBlackWhiteUpdateFromOTP)
  {
    _writeCommand(0x00); // panel setting
    _writeData(0x3f);    // partial update LUT from registers
    _writeCommand(0x82); // vcom_DC setting
    _writeData (0x30);   // -2.5V same value as in OTP
    _writeCommand(0x50); // VCOM AND DATA INTERVAL SETTING
    _writeData(0xA0);    // floating border
    _writeData(0x07);
    _writeCommand(0x20);
    _writeDataPGM(lut_20_LUTC_partial, sizeof(lut_20_LUTC_partial), 42 - sizeof(lut_20_LUTC_partial));
    _writeCommand(0x21);
    _writeDataPGM(lut_21_LUTWW_partial, sizeof(lut_21_LUTWW_partial), 42 - sizeof(lut_21_LUTWW_partial));
    _writeCommand(0x22);
    _writeDataPGM(lut_22_LUTKW_partial, sizeof(lut_22_LUTKW_partial), 42 - sizeof(lut_22_LUTKW_partial));
    _writeCommand(0x23);
    _writeDataPGM(lut_23_LUTWK_partial, sizeof(lut_23_LUTWK_partial), 42 - sizeof(lut_23_LUTWK_partial));
    _writeCommand(0x24);
    _writeDataPGM(lut_24_LUTKK_partial, sizeof(lut_24_LUTKK_partial), 42 - sizeof(lut_24_LUTKK_partial));
    _writeCommand(0x25);
    _writeDataPGM(lut_25_LUTBD_partial, sizeof(lut_25_LUTBD_partial), 42 - sizeof(lut_25_LUTBD_partial));
  }
  else
  {
    _writeCommand(0xE0);
    _writeData(0x02);
    _writeCommand(0xE5);
    _writeData(0x6E);
    _writeCommand(0x50); // VCOM AND DATA INTERVAL SETTING
    _writeData(0x80);    // floating border
    _writeData(0x07);
  }
  _PowerOn();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("refresh_bw", full_refresh_time);
  _writeCommand(0x92); // partial out
}
