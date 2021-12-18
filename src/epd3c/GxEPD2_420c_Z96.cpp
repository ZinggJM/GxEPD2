// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX.
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Panel: GDEH042Z96 : https://www.good-display.com/product/214.html
// Controller: SSD1619 : no link available currently, see https://www.good-display.cn/comp/xcompanyFile/downloadNew.do?appId=24&fid=322&id=309
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_420c_Z96.h"

GxEPD2_420c_Z96::GxEPD2_420c_Z96(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, HIGH, 50000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}

void GxEPD2_420c_Z96::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_420c_Z96::clearScreen(uint8_t black_value, uint8_t color_value)
{
  Serial.println("clearScreen");
  _initial_write = false; // initial full screen buffer clean done
  _Init_Part();
  _writeCommand(0x24);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(black_value);
  }
  _endTransfer();
  _writeCommand(0x26);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(~color_value);
  }
  _endTransfer();
  _Update_Part();
}

void GxEPD2_420c_Z96::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_420c_Z96::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  _initial_write = false; // initial full screen buffer clean done
  _Init_Part();
  _writeCommand(0x24);
  _startTransfer();
  for (uint16_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(black_value);
  }
  _endTransfer();
  _writeCommand(0x26);
  _startTransfer();
  for (uint16_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(~color_value);
  }
  _endTransfer();
  _writeCommand(0x92); // partial out
}

void GxEPD2_420c_Z96::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, NULL, x, y, w, h, invert, mirror_y, pgm);
} 

void GxEPD2_420c_Z96::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
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
  _Init_Part();
  //_setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x24);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (black)
      {
        // use wb, h of bitmap for index!
        int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
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
      _transfer(data);
    }
  }
  _endTransfer();
  _writeCommand(0x26);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (color)
      {
        // use wb, h of bitmap for index!
        int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
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
      _transfer(~data);
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_420c_Z96::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, NULL, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_420c_Z96::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
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
  if (!_using_partial_mode) _Init_Part();
  //_setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x24);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb_bitmap, h_bitmap of bitmap for index!
      int16_t idx = mirror_y ? x_part / 8 + j + dx / 8 + ((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + (y_part + i + dy) * wb_bitmap;
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
      _transfer(data);
    }
  }
  _endTransfer();
  _writeCommand(0x26);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data = 0xFF;
      if (color)
      {
        // use wb_bitmap, h_bitmap of bitmap for index!
        int16_t idx = mirror_y ? x_part / 8 + j + dx / 8 + ((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + (y_part + i + dy) * wb_bitmap;
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
      _transfer(~data);
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_420c_Z96::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_420c_Z96::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_Z96::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_Z96::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_Z96::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_Z96::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_420c_Z96::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else _Update_Full();
}

void GxEPD2_420c_Z96::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  _Init_Part();
  _Update_Part();
}

void GxEPD2_420c_Z96::powerOff()
{
  hibernate();
}

void GxEPD2_420c_Z96::hibernate()
{
  if (_rst >= 0)
  {
    _writeCommand(0x10); // deep sleep
    _writeData(0xA5);    // check code
    _hibernating = true;
  }
}

void GxEPD2_420c_Z96::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
}

void GxEPD2_420c_Z96::_InitDisplay()
{
  if (_hibernating) _reset();

  _waitWhileBusy("_InitDisplay1", init_time);
  _writeCommand(0x12);     //SWRESET
  _waitWhileBusy("_InitDisplay2", init_time);

  _writeCommand(0x74);
  _writeData(0x54);
  _writeCommand(0x7E);
  _writeData(0x3B);
  _writeCommand(0x2B);  // Reduce glitch under ACVCOM  
  _writeData(0x04);           
  _writeData(0x63);

  _writeCommand(0x0C);  // Soft start setting
  _writeData(0x8B);           
  _writeData(0x9C);
  _writeData(0x96);
  _writeData(0x0F);

  _writeCommand(0x01);  // Set MUX as 300
  _writeData(0x2B);           
  _writeData(0x01);
  _writeData(0x00);     

  _writeCommand(0x11);  // Data entry mode
  _writeData(0x01);         
  _writeCommand(0x44); 
  _writeData(0x00); // RAM x address start at 0
  _writeData(0x31); // RAM x address end at 31h(49+1)*8->400
  _writeCommand(0x45); 
  _writeData(0x2B);   // RAM y address start at 12Bh     
  _writeData(0x01);
  _writeData(0x00); // RAM y address end at 00h     
  _writeData(0x00);
  _writeCommand(0x3C); // board
  _writeData(0x01); // HIZ

  _writeCommand(0x18);
  _writeData(0X80);
  _writeCommand(0x22);
  _writeData(0XB1);  //Load Temperature and waveform setting.
  _writeCommand(0x20);
  _waitWhileBusy("_InitDisplay3", init_time);
  
  // set ram window
  _writeCommand(0x4E); 
  _writeData(0x00);
  _writeCommand(0x4F); 
  _writeData(0x2B);
  _writeData(0x01);
}

void GxEPD2_420c_Z96::_Init_Full()
{
  _InitDisplay();
}

void GxEPD2_420c_Z96::_Init_Part()
{
  _InitDisplay();
}

void GxEPD2_420c_Z96::_Update_Full()
{
  _writeCommand(0x22); //Display Update Control
  _writeData(0xC7);   
  _writeCommand(0x20);  //Activate Display Update Sequence
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_420c_Z96::_Update_Part()
{
  _writeCommand(0x22); //Display Update Control
  _writeData(0xC7);   
  _writeCommand(0x20);  //Activate Display Update Sequence
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}