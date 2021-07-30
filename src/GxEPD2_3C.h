// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_3C_H_
#define _GxEPD2_3C_H_
// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>

#ifndef ENABLE_GxEPD2_GFX
// default is off
#define ENABLE_GxEPD2_GFX 0
#endif

#if ENABLE_GxEPD2_GFX
#include "GxEPD2_GFX.h"
#define GxEPD2_GFX_BASE_CLASS GxEPD2_GFX
#elif defined(_GFX_H_)
#define GxEPD2_GFX_BASE_CLASS GFX
#else
#include <Adafruit_GFX.h>
#define GxEPD2_GFX_BASE_CLASS Adafruit_GFX
#endif

#include "GxEPD2_EPD.h"
#include "epd3c/GxEPD2_154c.h"
#include "epd3c/GxEPD2_154_Z90c.h"
#include "epd3c/GxEPD2_213c.h"
#include "epd3c/GxEPD2_213_Z19c.h"
#include "epd3c/GxEPD2_290c.h"
#include "epd3c/GxEPD2_290_Z13c.h"
#include "epd3c/GxEPD2_290_C90c.h"
#include "epd3c/GxEPD2_270c.h"
#include "epd3c/GxEPD2_420c.h"
#include "epd3c/GxEPD2_583c.h"
#include "epd3c/GxEPD2_565c.h"
#include "epd3c/GxEPD2_750c.h"
#include "epd3c/GxEPD2_750c_Z08.h"
#include "epd3c/GxEPD2_750c_Z90.h"

template<typename GxEPD2_Type, const uint16_t page_height>
class GxEPD2_3C : public GxEPD2_GFX_BASE_CLASS
{
  public:
    GxEPD2_Type epd2;
#if ENABLE_GxEPD2_GFX
    GxEPD2_3C(GxEPD2_Type epd2_instance) : GxEPD2_GFX_BASE_CLASS(epd2, GxEPD2_Type::WIDTH, GxEPD2_Type::HEIGHT), epd2(epd2_instance)
#else
    GxEPD2_3C(GxEPD2_Type epd2_instance) : GxEPD2_GFX_BASE_CLASS(GxEPD2_Type::WIDTH, GxEPD2_Type::HEIGHT), epd2(epd2_instance)
#endif
    {
      _page_height = page_height;
      _pages = (HEIGHT / _page_height) + ((HEIGHT % _page_height) > 0);
      _mirror = false;
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    uint16_t pages()
    {
      return _pages;
    }

    uint16_t pageHeight()
    {
      return _page_height;
    }

    bool mirror(bool m)
    {
      _swap_ (_mirror, m);
      return m;
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
      if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;
      if (_mirror) x = width() - x - 1;
      // check rotation, move pixel around if necessary
      switch (getRotation())
      {
        case 1:
          _swap_(x, y);
          x = WIDTH - x - 1;
          break;
        case 2:
          x = WIDTH - x - 1;
          y = HEIGHT - y - 1;
          break;
        case 3:
          _swap_(x, y);
          y = HEIGHT - y - 1;
          break;
      }
      // transpose partial window to 0,0
      x -= _pw_x;
      y -= _pw_y;
      // clip to (partial) window
      if ((x < 0) || (x >= _pw_w) || (y < 0) || (y >= _pw_h)) return;
      // adjust for current page
      y -= _current_page * _page_height;
      // check if in current page
      if ((y < 0) || (y >= _page_height)) return;
      uint16_t i = x / 8 + y * (_pw_w / 8);
      _black_buffer[i] = (_black_buffer[i] | (1 << (7 - x % 8))); // white
      _color_buffer[i] = (_color_buffer[i] | (1 << (7 - x % 8)));
      if (color == GxEPD_WHITE) return;
      else if (color == GxEPD_BLACK) _black_buffer[i] = (_black_buffer[i] & (0xFF ^ (1 << (7 - x % 8))));
      else if ((color == GxEPD_RED) || (color == GxEPD_YELLOW)) _color_buffer[i] = (_color_buffer[i] & (0xFF ^ (1 << (7 - x % 8))));
    }

    void init(uint32_t serial_diag_bitrate = 0) // = 0 : disabled
    {
      epd2.init(serial_diag_bitrate);
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    // init method with additional parameters:
    // initial false for re-init after processor deep sleep wake up, if display power supply was kept
    // only relevant for b/w displays with fast partial update
    // reset_duration = 20 is default; a value of 2 may help with "clever" reset circuit of newer boards from Waveshare
    // pulldown_rst_mode true for alternate RST handling to avoid feeding 5V through RST pin
    void init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration = 20, bool pulldown_rst_mode = false)
    {
      epd2.init(serial_diag_bitrate, initial, reset_duration, pulldown_rst_mode);
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    void fillScreen(uint16_t color) // 0x0 black, >0x0 white, to buffer
    {
      uint8_t black = 0xFF;
      uint8_t red = 0xFF;
      if (color == GxEPD_WHITE);
      else if (color == GxEPD_BLACK) black = 0x00;
      else if ((color == GxEPD_RED) || (color == GxEPD_YELLOW)) red = 0x00;
      for (uint16_t x = 0; x < sizeof(_black_buffer); x++)
      {
        _black_buffer[x] = black;
        _color_buffer[x] = red;
      }
    }

    // display buffer content to screen, useful for full screen buffer
    void display(bool partial_update_mode = false)
    {
      epd2.writeImage(_black_buffer, _color_buffer, 0, 0, WIDTH, _page_height);
      epd2.refresh(partial_update_mode);
      if (!partial_update_mode) epd2.powerOff();
    }

    // display part of buffer content to screen, useful for full screen buffer
    // displayWindow, use parameters according to actual rotation.
    // x and w should be multiple of 8, for rotation 0 or 2,
    // y and h should be multiple of 8, for rotation 1 or 3,
    // else window is increased as needed,
    // this is an addressing limitation of the e-paper controllers
    void displayWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      x = gx_uint16_min(x, width());
      y = gx_uint16_min(y, height());
      w = gx_uint16_min(w, width() - x);
      h = gx_uint16_min(h, height() - y);
      _rotate(x, y, w, h);
      epd2.writeImagePart(_black_buffer, _color_buffer, x, y, WIDTH, _page_height, x, y, w, h);
      epd2.refresh(x, y, w, h);
    }

    void displayWindowBW(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      x = gx_uint16_min(x, width());
      y = gx_uint16_min(y, height());
      w = gx_uint16_min(w, width() - x);
      h = gx_uint16_min(h, height() - y);
      _rotate(x, y, w, h);
      epd2.writeImagePartNew(_black_buffer, x, y, WIDTH, _page_height, x, y, w, h);
      epd2.refresh_bw(x, y, w, h);
      epd2.writeImagePartPrevious(_black_buffer, x, y, WIDTH, _page_height, x, y, w, h);
    }

    void setFullWindow()
    {
      _using_partial_mode = false;
      _pw_x = 0;
      _pw_y = 0;
      _pw_w = WIDTH;
      _pw_h = HEIGHT;
    }

    // setPartialWindow, use parameters according to actual rotation.
    // x and w should be multiple of 8, for rotation 0 or 2,
    // y and h should be multiple of 8, for rotation 1 or 3,
    // else window is increased as needed,
    // this is an addressing limitation of the e-paper controllers
    void setPartialWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      if (!epd2.hasPartialUpdate) return;
      _pw_x = gx_uint16_min(x, width());
      _pw_y = gx_uint16_min(y, height());
      _pw_w = gx_uint16_min(w, width() - _pw_x);
      _pw_h = gx_uint16_min(h, height() - _pw_y);
      _rotate(_pw_x, _pw_y, _pw_w, _pw_h);
      _using_partial_mode = true;
      // make _pw_x, _pw_w multiple of 8
      _pw_w += _pw_x % 8;
      if (_pw_w % 8 > 0) _pw_w += 8 - _pw_w % 8;
      _pw_x -= _pw_x % 8;
    }

    void firstPage()
    {
      fillScreen(GxEPD_WHITE);
      _current_page = 0;
      _second_phase = false;
      epd2.setPaged(); // for GxEPD2_154c paged workaround
    }

    bool nextPage()
    {
      uint16_t page_ys = _current_page * _page_height;
      if (_using_partial_mode)
      {
        //Serial.print("  nextPage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(_pw_y); Serial.print(", ");
        //Serial.print(_pw_w); Serial.print(", "); Serial.print(_pw_h); Serial.print(") P"); Serial.println(_current_page);
        uint16_t page_ye = _current_page < (_pages - 1) ? page_ys + _page_height : HEIGHT;
        uint16_t dest_ys = _pw_y + page_ys; // transposed
        uint16_t dest_ye = gx_uint16_min(_pw_y + _pw_h, _pw_y + page_ye);
        if (dest_ye > dest_ys)
        {
          //Serial.print("writeImage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(dest_ys); Serial.print(", ");
          //Serial.print(_pw_w); Serial.print(", "); Serial.print(dest_ye - dest_ys); Serial.println(")");
          epd2.writeImage(_black_buffer, _color_buffer, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
        }
        else
        {
          //Serial.print("writeImage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(dest_ys); Serial.print(", ");
          //Serial.print(_pw_w); Serial.print(", "); Serial.print(dest_ye - dest_ys); Serial.print(") skipped ");
          //Serial.print(dest_ys); Serial.print(".."); Serial.println(dest_ye);
        }
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          if (!_second_phase)
          {
            epd2.refresh(_pw_x, _pw_y, _pw_w, _pw_h);
            if (epd2.hasFastPartialUpdate)
            {
              _second_phase = true;
              return true;
            }
          }
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
      else // full update
      {
        epd2.writeImage(_black_buffer, _color_buffer, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          if ((epd2.panel == GxEPD2::GDEW0154Z04) && (_pages > 1))
          {
            if (!_second_phase)
            {
              epd2.refresh(false); // full update after first phase
              _second_phase = true;
              fillScreen(GxEPD_WHITE);
              return true;
            }
            else epd2.refresh(true); // partial update after second phase
          } else epd2.refresh(false); // full update after only phase
          epd2.powerOff();
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
    }

    bool nextPageBW()
    {
      if (1 == _pages)
      {
        if (_using_partial_mode)
        {
          epd2.writeImageNew(_black_buffer, _pw_x, _pw_y, _pw_w, _pw_h);
          epd2.refresh_bw(_pw_x, _pw_y, _pw_w, _pw_h);
          epd2.writeImagePrevious(_black_buffer, _pw_x, _pw_y, _pw_w, _pw_h);
        }
        else // full update
        {
          epd2.writeImage(_black_buffer, 0, 0, WIDTH, HEIGHT);
          epd2.refresh(false);
          epd2.writeImagePrevious(_black_buffer, 0, 0, WIDTH, HEIGHT);
          epd2.powerOff();
        }
        return false;
      }
      uint16_t page_ys = _current_page * _page_height;
      if (_using_partial_mode)
      {
        //Serial.print("  nextPage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(_pw_y); Serial.print(", ");
        //Serial.print(_pw_w); Serial.print(", "); Serial.print(_pw_h); Serial.print(") P"); Serial.println(_current_page);
        uint16_t page_ye = _current_page < (_pages - 1) ? page_ys + _page_height : HEIGHT;
        uint16_t dest_ys = _pw_y + page_ys; // transposed
        uint16_t dest_ye = gx_uint16_min(_pw_y + _pw_h, _pw_y + page_ye);
        if (dest_ye > dest_ys)
        {
          //Serial.print("writeImage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(dest_ys); Serial.print(", ");
          //Serial.print(_pw_w); Serial.print(", "); Serial.print(dest_ye - dest_ys); Serial.println(")");
          if (!_second_phase) epd2.writeImageNew(_black_buffer, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
          else epd2.writeImagePrevious(_black_buffer, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
        }
        else
        {
          //Serial.print("writeImage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(dest_ys); Serial.print(", ");
          //Serial.print(_pw_w); Serial.print(", "); Serial.print(dest_ye - dest_ys); Serial.print(") skipped ");
          //Serial.print(dest_ys); Serial.print(".."); Serial.println(dest_ye);
        }
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          if (!_second_phase)
          {
            epd2.refresh_bw(_pw_x, _pw_y, _pw_w, _pw_h);
            _second_phase = true;
            fillScreen(GxEPD_WHITE);
            return true;
          }
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
      else // full update
      {
        if (!_second_phase) epd2.writeImage(_black_buffer, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        else epd2.writeImagePrevious(_black_buffer, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        _current_page++;
        if (_current_page == _pages)
        {
          _current_page = 0;
          if (!_second_phase)
          {
            epd2.refresh(false); // full update after first phase
            _second_phase = true;
            fillScreen(GxEPD_WHITE);
            return true;
          }
          epd2.powerOff();
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
    }

    // GxEPD style paged drawing; drawCallback() is called as many times as needed
    void drawPaged(void (*drawCallback)(const void*), const void* pv)
    {
      if (_using_partial_mode)
      {
        for (_current_page = 0; _current_page < _pages; _current_page++)
        {
          uint16_t page_ys = _current_page * _page_height;
          uint16_t page_ye = _current_page < (_pages - 1) ? page_ys + _page_height : HEIGHT;
          uint16_t dest_ys = _pw_y + page_ys; // transposed
          uint16_t dest_ye = gx_uint16_min(_pw_y + _pw_h, _pw_y + page_ye);
          if (dest_ye > dest_ys)
          {
            fillScreen(GxEPD_WHITE);
            drawCallback(pv);
            epd2.writeImage(_black_buffer, _color_buffer, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
          }
        }
        epd2.refresh(_pw_x, _pw_y, _pw_w, _pw_h);
      }
      else // full update
      {
        epd2.setPaged(); // for GxEPD2_154c paged workaround
        for (_current_page = 0; _current_page < _pages; _current_page++)
        {
          uint16_t page_ys = _current_page * _page_height;
          fillScreen(GxEPD_WHITE);
          drawCallback(pv);
          epd2.writeImage(_black_buffer, _color_buffer, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        }
        if (epd2.panel == GxEPD2::GDEW0154Z04)
        { // GxEPD2_154c paged workaround: write color part
          for (_current_page = 0; _current_page < _pages; _current_page++)
          {
            uint16_t page_ys = _current_page * _page_height;
            fillScreen(GxEPD_WHITE);
            drawCallback(pv);
            epd2.writeImage(_black_buffer, _color_buffer, 0, page_ys, WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
          }
        }
        epd2.refresh(false); // full update
        epd2.powerOff();
      }
      _current_page = 0;
    }

    void drawInvertedBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
    {
      // taken from Adafruit_GFX.cpp, modified
      int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
      uint8_t byte = 0;
      for (int16_t j = 0; j < h; j++)
      {
        for (int16_t i = 0; i < w; i++ )
        {
          if (i & 7) byte <<= 1;
          else
          {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
            byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
#else
            byte = bitmap[j * byteWidth + i / 8];
#endif
          }
          if (!(byte & 0x80))
          {
            drawPixel(x + i, y + j, color);
          }
        }
      }
    }

    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    void clearScreen(uint8_t value = 0xFF) // init controller memory and screen (default white)
    {
      epd2.clearScreen(value);
    }
    void writeScreenBuffer(uint8_t value = 0xFF) // init controller memory (default white)
    {
      epd2.writeScreenBuffer(value);
    }
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.writeImage(black, color, x, y, w, h, false, false, false);
    }
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, false, false, false);
    }
    // write sprite of native data to controller memory, without screen refresh; x and w should be multiple of 8
    void writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
    }
    // write to controller memory, with screen refresh; x and w should be multiple of 8
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.drawImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.drawImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.drawImage(black, color, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.drawImage(black, color, x, y, w, h, false, false, false);
    }
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.drawImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.drawImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, false, false, false);
    }
    // write sprite of native data to controller memory, with screen refresh; x and w should be multiple of 8
    void drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.drawNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
    }
    void refresh(bool partial_update_mode = false) // screen refresh from controller memory to full screen
    {
      epd2.refresh(partial_update_mode);
      if (!partial_update_mode) epd2.powerOff();
    }
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h) // screen refresh from controller memory, partial screen
    {
      epd2.refresh(x, y, w, h);
    }
    // turns off generation of panel driving voltages, avoids screen fading over time
    void powerOff()
    {
      epd2.powerOff();
    }
    // turns powerOff() and sets controller to deep sleep for minimum power use, ONLY if wakeable by RST (rst >= 0)
    void hibernate()
    {
      epd2.hibernate();
    }
  private:
    template <typename T> static inline void
    _swap_(T & a, T & b)
    {
      T t = a;
      a = b;
      b = t;
    };
    static inline uint16_t gx_uint16_min(uint16_t a, uint16_t b)
    {
      return (a < b ? a : b);
    };
    static inline uint16_t gx_uint16_max(uint16_t a, uint16_t b)
    {
      return (a > b ? a : b);
    };
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
    {
      switch (getRotation())
      {
        case 1:
          _swap_(x, y);
          _swap_(w, h);
          x = WIDTH - x - w;
          break;
        case 2:
          x = WIDTH - x - w;
          y = HEIGHT - y - h;
          break;
        case 3:
          _swap_(x, y);
          _swap_(w, h);
          y = HEIGHT - y - h;
          break;
      }
    }
  private:
    uint8_t _black_buffer[(GxEPD2_Type::WIDTH / 8) * page_height];
    uint8_t _color_buffer[(GxEPD2_Type::WIDTH / 8) * page_height];
    bool _using_partial_mode, _second_phase, _mirror;
    uint16_t _width_bytes, _pixel_bytes;
    int16_t _current_page;
    uint16_t _pages, _page_height;
    uint16_t _pw_x, _pw_y, _pw_w, _pw_h;
};

#endif
