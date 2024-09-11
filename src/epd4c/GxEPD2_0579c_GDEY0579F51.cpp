// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display:https://www.good-display.com/comp/xcompanyFile/downloadNew.do?appId=24&fid=2134&id=1555
// Panel: GDEY0579F51 : https://www.good-display.com/product/570.html
// Panel specs: https://v4.cecdn.yun300.cn/100001_1909185148/GDEY0579F51.pdf
// Controller: HX8717 : https://v4.cecdn.yun300.cn/100001_1909185148/HX8717-A_DS_preliminary_v01_230417.pdf
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_0579c_GDEY0579F51.h"

GxEPD2_0579c_GDEY0579F51::GxEPD2_0579c_GDEY0579F51(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 50000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
  _paged = false;
  _needs_re_init = false;
}

void GxEPD2_0579c_GDEY0579F51::clearScreen(uint8_t value)
{
  //Serial.println("clearScreen");
  clearScreen(value, 0xFF);
}

void GxEPD2_0579c_GDEY0579F51::clearScreen(uint8_t black_value, uint8_t color_value)
{
  //Serial.println("clearScreen");
  if (_needs_re_init) _InitDisplay();
  writeScreenBuffer(black_value, color_value);
  refresh();
}

void GxEPD2_0579c_GDEY0579F51::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_0579c_GDEY0579F51::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  //Serial.println("writeScreenBuffer");
  if (!_init_display_done) _InitDisplay();
  _setPartialRamAreaMaster(0, 0, WIDTH / 2, HEIGHT, false);
  _writeCommand(0x10);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 4; i++)
  {
    _transfer(0xFF == black_value ? 0x55 : 0x00);
  }
  _endTransfer();
  _setPartialRamAreaSlave(0, 0, WIDTH / 2, HEIGHT, false);
  _writeCommand(0x10);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH / 2) * uint32_t(HEIGHT) / 4; i++)
  {
    _transfer(0xFF == black_value ? 0x55 : 0x00);
  }
  _endTransfer();
  _initial_write = false; // initial full screen buffer clean done
}

void GxEPD2_0579c_GDEY0579F51::_writeLineFromImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t wbb, bool invert, bool pgm, bool lowside, bool trf)
{
  bool skip_first = (w % 8 != 0) && !lowside;
  bool skip_last = (w % 8 != 0) && lowside;
  if (trf) _startTransfer();
  for (int16_t j = 0; j < w; j += 8)
  {
    uint8_t black_data = 0xFF, color_data = 0xFF;
    // use wbb, h of bitmap for index!
    uint32_t idx = uint32_t(x + j) / 8 + uint32_t(y) * wbb;
    if (pgm)
    {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
      if (black) black_data = pgm_read_byte(&black[idx]);
      if (color) color_data = pgm_read_byte(&color[idx]);
#else
      if (black) black_data = black[idx];
      if (color) color_data = color[idx];
#endif
    }
    else
    {
      if (black) black_data = black[idx];
      if (color) color_data = color[idx];
    }
    if (invert)
    {
      black_data = ~black_data;
      color_data = ~color_data;
    }
    for (int16_t k = 0; k < 2; k++)
    {
      uint8_t out_data = 0x00;
      for (int16_t l = 0; l < 4; l++)
      {
        out_data <<= 2;
        if (!(color_data & 0x80)) out_data |= 0x03; // red
        else out_data |= black_data & 0x80 ? 0x01 : 0x00; // white or black
        black_data <<= 1;
        color_data <<= 1;
      }
      if (!(skip_first && (j == 0) && (k == 0)) && !(skip_last && (j + 8 >= w) && (k == 1))) _transfer(out_data);
    }
  }
  if (trf) _endTransfer();
}

void GxEPD2_0579c_GDEY0579F51::_writeDataFromImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, int16_t wbb, int16_t hb, bool invert, bool mirror_y, bool pgm, bool lowside)
{
  _startTransfer();
  for (int16_t i = 0; i < h; i++)
  {
    int16_t y1 = !mirror_y ? hb - 1 - (i + y) : i + y;
    if (h == 1) y1 = i + y;
    _writeLineFromImage(black, color, x, y1, w, wbb, invert, pgm, lowside, false);
    if (h > 1)
    {
      int16_t y1 = mirror_y ? hb - 1 - (i + y) : i + y;
      _writeLineFromImage(black, color, x, y1, w, wbb, invert, pgm, lowside, false);
    }
  }
  _endTransfer();
}

void GxEPD2_0579c_GDEY0579F51::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, 0, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_0579c_GDEY0579F51::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!black && !color) return;
  //Serial.print("writeImage("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  if (_needs_re_init) _ReInitDisplay();
  int16_t wbb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wbb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;

  y1 = HEIGHT - h1 - y1; // flip for controller

  if ((x1 < WIDTH / 2) && true) // MasterSide ****
  {
    int16_t pw_w = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    int16_t pw_x = x1;
    if (h1 == HEIGHT)
    {
      _setPartialRamAreaMaster(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromImage(black, color, dx, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm, true);
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h - 1 - dy - i : dy + i;
        if (((y1 + i) % 2 == 0) && ((y1 + i) < HEIGHT / 2))
        { // shows at 0, half height
          int16_t pw_y = 2 * (y1 + i);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
      }
    }
  }
  if ((x1 + w1 > WIDTH / 2) && true) // SlaveSide ****
  {
    int16_t pw_w = x1 <= WIDTH / 2 ? x1 + w1 - WIDTH / 2 : w1;
    int16_t pw_x = x1 <= WIDTH / 2 ? 0 : x1 - WIDTH / 2;
    int16_t xbm = dx + (w1 - pw_w);
    if ((h1 == HEIGHT) && true)
    {
      _setPartialRamAreaSlave(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromImage(black, color, xbm, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm, false);
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h - 1 - dy - i : dy + i;
        if (((i + y1) % 2 == 0) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
      }
    }
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_0579c_GDEY0579F51::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, 0, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_0579c_GDEY0579F51::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeImagePart("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", ");
  //Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  if (!black && !color) return;
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  if (_needs_re_init) _ReInitDisplay();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wbb = (w_bitmap + 7) / 8; // width bytes, bitmaps are padded
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
  dx += x_part;
  dy += y_part;
  if ((w1 <= 0) || (h1 <= 0)) return;

  // next line commented out for demo picture
  //y1 = HEIGHT - h1 - y1; // flip for controller

  if ((x1 < WIDTH / 2) && true) // MasterSide ****
  {
    int16_t pw_w = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    int16_t pw_x = x1;
    if (h1 == HEIGHT)
    {
      _setPartialRamAreaMaster(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromImage(black, color, dx, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm, true);
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h_bitmap - 1 - dy - i : dy + i;
        if (((y1 + i) % 2 == 0) && ((y1 + i) < HEIGHT / 2))
        { // shows at 0, half height
          int16_t pw_y = 2 * (y1 + i);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, dx, ybm, pw_w, wbb, invert, pgm, true, true);
        }
      }
    }
  }
  if ((x1 + w1 > WIDTH / 2) && true) // SlaveSide ****
  {
    int16_t pw_w = x1 <= WIDTH / 2 ? x1 + w1 - WIDTH / 2 : w1;
    int16_t pw_x = x1 <= WIDTH / 2 ? 0 : x1 - WIDTH / 2;
    if ((h1 == HEIGHT) && true)
    {
      _setPartialRamAreaSlave(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromImage(black, color, x_part + WIDTH / 2, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm, false);
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h_bitmap - 1 - dy - i : dy + i;
        int16_t xbm = dx + (w1 - pw_w);
        if (((i + y1) % 2 == 0) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromImage(black, color, xbm, ybm, pw_w, wbb, invert, pgm, false, true);
        }
      }
    }
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_0579c_GDEY0579F51::_writeLineFromNative(const uint8_t* native, int16_t x, int16_t y, int16_t w, int16_t wbb, bool invert, bool pgm, bool trf)
{
  if (trf) _startTransfer();
  for (int16_t j = 0; j < w; j += 4)
  {
    uint8_t data;
    // use wbb, h of bitmap for index!
    uint32_t idx = uint32_t(x + j) / 4 + uint32_t(y) * wbb;
    if (pgm)
    {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
      data = pgm_read_byte(&native[idx]);
#else
      data = native[idx];
#endif
    }
    else
    {
      data = native[idx];
    }
    if (invert) data = ~data;
    _transfer(data);
  }
  if (trf) _endTransfer();
}

void GxEPD2_0579c_GDEY0579F51::_writeDataFromNative(const uint8_t* native, int16_t x, int16_t y, int16_t w, int16_t h, int16_t wbb, int16_t hb, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("_writeDataFromNative("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.print(", ");
  //Serial.print(wbb); Serial.print(", "); Serial.print(hb); Serial.print(", "); Serial.println(")");
  _startTransfer();
  for (int16_t i = 0; i < h; i++)
  {
    int16_t y1 = !mirror_y ? hb - 1 - (i + y) : i + y;
    _writeLineFromNative(native, x, y1, w, wbb, invert, pgm, false);
    if (h > 1)
    {
      int16_t y1 = mirror_y ? hb - 1 - (i + y) : i + y;
      _writeLineFromNative(native, x, y1, w, wbb, invert, pgm, false);
    }
  }
  _endTransfer();
}

void GxEPD2_0579c_GDEY0579F51::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (!data1) return;
  //Serial.print("writeNative("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  if (_needs_re_init) _ReInitDisplay();
  int16_t wbb = (w + 3) / 4; // width bytes, bitmaps are padded
  x -= x % 4; // byte boundary
  w = wbb * 4; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;

  y1 = HEIGHT - h1 - y1; // flip for controller

  if ((x1 < WIDTH / 2) && true) // MasterSide ****
  {
    int16_t pw_w = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    int16_t pw_x = x1;
    if ((h1 == HEIGHT) && true)
    {
      _setPartialRamAreaMaster(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromNative(data1, dx, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm );
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h - 1 - dy - i : dy + i;
        if (((y1 + i) % 2 == 0) && ((y1 + i) < HEIGHT / 2))
        { // shows at 0, half height
          int16_t pw_y = 2 * (y1 + i);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
      }
    }
  }
  if ((x1 + w1 > WIDTH / 2) && true) // SlaveSide ****
  {
    int16_t pw_w = x1 <= WIDTH / 2 ? x1 + w1 - WIDTH / 2 : w1;
    int16_t pw_x = x1 <= WIDTH / 2 ? 0 : x1 - WIDTH / 2;
    int16_t xbm = dx + (w1 - pw_w);
    if ((h1 == HEIGHT) && true)
    {
      _setPartialRamAreaSlave(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromNative(data1, xbm, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm);
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h - 1 - dy - i : dy + i;
        if (((i + y1) % 2 == 0) && ((i + y1) < HEIGHT / 2) && (true))
        { // shows at 0, half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2) && (true))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
      }
    }
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_0579c_GDEY0579F51::writeNativePart(const uint8_t* data1, const uint8_t* data2, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  //Serial.print("writeNativePart("); Serial.print(x_part); Serial.print(", "); Serial.print(y_part); Serial.print(", ");
  //Serial.print(w_bitmap); Serial.print(", "); Serial.print(h_bitmap); Serial.print(", ");
  //Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  if (!data1) return;
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
  if (!_init_display_done) _InitDisplay();
  if (_initial_write) writeScreenBuffer();
  if (_needs_re_init) _ReInitDisplay();
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wbb = (w_bitmap + 3) / 4; // width bytes, bitmaps are padded
  x_part -= x_part % 4; // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w; // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h; // limit
  w += x % 4; // adjust for byte boundary of x
  x -= x % 4; // byte boundary
  w = 4 * ((w + 3) / 4); // byte boundary, bitmaps are padded
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  dx += x_part;
  dy += y_part;
  if ((w1 <= 0) || (h1 <= 0)) return;

  // next line commented out for demo picture
  //y1 = HEIGHT - h1 - y1; // flip for controller

  if ((x1 < WIDTH / 2) && true) // MasterSide ****
  {
    int16_t pw_w = x1 + w1 <= WIDTH / 2 ? w1 : WIDTH / 2 - x1;
    int16_t pw_x = x1;
    if ((h1 == HEIGHT) && true)
    {
      _setPartialRamAreaSlave(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromNative(data1, dx, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm );
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h_bitmap - 1 - dy - i : dy + i;
        if (((y1 + i) % 2 == 0) && ((y1 + i) < HEIGHT / 2))
        { // shows at 0, half height
          int16_t pw_y = 2 * (y1 + i);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaMaster(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, dx, ybm, pw_w, wbb, invert, pgm, true);
        }
      }
    }
  }
  if ((x1 + w1 > WIDTH / 2) && true) // SlaveSide ****
  {
    int16_t pw_w = x1 <= WIDTH / 2 ? x1 + w1 - WIDTH / 2 : w1;
    int16_t pw_x = x1 <= WIDTH / 2 ? 0 : x1 - WIDTH / 2;
    int16_t xbm = dx + (w1 - pw_w);
    if ((h1 == HEIGHT) && true)
    {
      _setPartialRamAreaSlave(pw_x, 0, pw_w, HEIGHT);
      _writeCommand(0x10);
      _writeDataFromNative(data1, xbm, dy, pw_w, h1, wbb, h, invert, mirror_y, pgm);
    }
    else
    {
      for (int16_t i = 0; i < h1; i++)
      {
        int16_t ybm = !mirror_y ? h_bitmap - 1 - dy - i : dy + i;
        if (((i + y1) % 2 == 0) && ((i + y1) < HEIGHT / 2) && (true))
        { // shows at 0, half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) < HEIGHT / 2) && (true))
        { // shows at 0, half height, completes half height
          int16_t pw_y = 2 * (i + y1);
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 0) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
        if (((i + y1) % 2 == 1) && ((i + y1) >= HEIGHT / 2) && (true))
        { // shows at half height the right part
          int16_t pw_y = 2 * (HEIGHT - y1 - i) + 1;
          _setPartialRamAreaSlave(pw_x, pw_y, pw_w, 1);
          _writeCommand(0x10);
          _writeLineFromNative(data1, xbm, ybm, pw_w, wbb, invert, pgm, true);
        }
      }
    }
  }
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_0579c_GDEY0579F51::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_0579c_GDEY0579F51::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_0579c_GDEY0579F51::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_0579c_GDEY0579F51::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_0579c_GDEY0579F51::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_0579c_GDEY0579F51::refresh(bool partial_update_mode)
{
  _setPartialRamAreaBoth(0, 0, WIDTH / 2, HEIGHT);
  _refresh(partial_update_mode);
}

void GxEPD2_0579c_GDEY0579F51::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
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
  //if (hasPartialUpdate) _setPartialRamArea(x1, y1, w1, h1, !fullscreen);
  _setPartialRamAreaBoth(0, 0, WIDTH / 2, HEIGHT);
  _refresh(!fullscreen);
}

void GxEPD2_0579c_GDEY0579F51::powerOff()
{
  _PowerOff();
}

void GxEPD2_0579c_GDEY0579F51::hibernate()
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

void GxEPD2_0579c_GDEY0579F51::setPaged()
{
  if (!hasPartialUpdate)
  {
    _paged = true;
    _InitDisplay();
    _writeCommand(0x10);
  }
}

void GxEPD2_0579c_GDEY0579F51::_refresh(bool partial_update_mode)
{
  _writeCommand(0x50); // VCOM and Data Interval Setting
  _writeData(partial_update_mode ? 0x97 : 0x37); // floating or white
  _PowerOn();
  _writeCommand(0x12); // Display Refresh
  _writeData(0x00);
  delay(1);
  _waitWhileBusy("_refresh", full_refresh_time);
  _setPartialRamAreaBoth(0, 0, WIDTH / 2, HEIGHT);
  _needs_re_init = true;
}

void GxEPD2_0579c_GDEY0579F51::_setPartialRamAreaMaster(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial_mode)
{
  //Serial.print("_setPartialRamAreaMaster("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _setPartialRamArea(x, y, w, h, 0x01, partial_mode);
}
void GxEPD2_0579c_GDEY0579F51::_setPartialRamAreaSlave(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial_mode)
{
  //Serial.print("_setPartialRamAreaSlave("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _setPartialRamArea(x, y, w, h, 0x02, partial_mode);
}
void GxEPD2_0579c_GDEY0579F51::_setPartialRamAreaBoth(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial_mode)
{
  //Serial.print("_setPartialRamAreaBoth("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _setPartialRamArea(x, y, w, h, 0x00, partial_mode);
}

void GxEPD2_0579c_GDEY0579F51::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t select, bool partial_mode)
{
  //Serial.print("_setPartialRamArea("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", ");
  //Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _writeCommand(0xA2); // select
  _writeData(select);
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

void GxEPD2_0579c_GDEY0579F51::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_0579c_GDEY0579F51::_PowerOff()
{
  if (_power_is_on)
  {
    _writeCommand(0x02);
    _writeData(0x00);
    _waitWhileBusy("_PowerOff", power_off_time);
  }
  _power_is_on = false;
}

void GxEPD2_0579c_GDEY0579F51::_InitDisplay()
{
  //Serial.println("_InitDisplay");
  // initcode from Good Dispay demo for GDEY0579F51
  if ((_rst >= 0) && (_hibernating || _initial_write))
  {
    digitalWrite(_rst, HIGH);
    delay(20); // At least 20ms delay
    digitalWrite(_rst, LOW); // Module reset
    delay(2);  // At least 40ms delay, 2ms for WS "clever" reset
    digitalWrite(_rst, HIGH);
    delay(10); // At least 10ms delay (3ms measured)
    //_waitWhileBusy("_InitDisplay reset", power_on_time);
    _hibernating = false;
    _power_is_on = false;
  }
  _writeCommand(0xA2); // select
  _writeData(0x01);    // master
  _writeCommand(0x00); // PSR
  _writeData(0x07);
  _writeData(0x29);
  _writeCommand(0xA2); // select
  _writeData(0x02);    // slave
  _writeCommand(0x00); // PSR
  _writeData(0x03);
  _writeData(0x29);
  _writeCommand(0xA2); // select
  _writeData(0x00);    // master & slave
  _writeCommand(0x06); // BTST
  _writeData(0x32);
  _writeData(0x32);
  _writeData(0x32);
  _writeData(0x32);
  _writeCommand(0x61); //TRES
  _writeData((WIDTH / 2) / 256); // Source_BITS_H
  _writeData((WIDTH / 2) % 256); // Source_BITS_L
  _writeData(HEIGHT / 256); // Gate_BITS_H
  _writeData(HEIGHT % 256); // Gate_BITS_L
  _writeCommand(0x50); // CDI
  _writeData(0x97);    // border white
  _writeCommand(0xE9);
  _writeData(0x01);
  _writeCommand(0xE0);
  _writeData(0x01);
  _writeCommand(0xA2); // select
  _writeData(0x01);    // master
  _PowerOn();
  _init_display_done = true;
  _setPartialRamAreaBoth(0, 0, WIDTH / 2, HEIGHT);
}

void GxEPD2_0579c_GDEY0579F51::_ReInitDisplay()
{
  //Serial.println("_ReInitDisplay");
  _writeCommand(0x50); // CDI
  _writeData(0x97);    // border white
  _writeCommand(0xE9);
  _writeData(0x01);
  _writeCommand(0xE0);
  _writeData(0x01);
  _writeCommand(0xA2); // select
  _writeData(0x01);    // master
  _PowerOn();
  writeScreenBuffer(); // clean up the mess
  _init_display_done = true;
  _needs_re_init = false;
}
