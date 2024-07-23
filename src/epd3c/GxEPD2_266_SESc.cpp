// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: https://www.good-display.com/comp/xcompanyFile/downloadNew.do?appId=24&fid=1253&id=791
// Panel: FE2266FS094 (from VUSION Electronic Shelf Labels) : http://pricetags.ru/wp-content/uploads/2018/04/datasheet_ESL_VUSION_26BWR.pdf
// Controller: UC8253 : https://v4.cecdn.yun300.cn/100001_2104235159/UC8253.pdf
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2
//
// Contributed by Zxis233

#include "GxEPD2_266_SESc.h"

#define isEPD_W21_BUSY digitalRead(_busy)

GxEPD2_266_SESc::GxEPD2_266_SESc(int16_t cs, int16_t dc, int16_t rst, int16_t busy) : GxEPD2_EPD(cs, dc, rst, busy, HIGH, 30000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}

void GxEPD2_266_SESc::clearScreen(uint8_t value)
{
    clearScreen(value, 0xFF);
}

void GxEPD2_266_SESc::clearScreen(uint8_t black_value, uint8_t color_value)
{
    _initial_write = false;  // initial full screen buffer clean done
    _Init_Part();
    _setPartialRamArea(0, 0, WIDTH, HEIGHT);
    _writeCommand(0x10);
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
        _writeData(black_value);
    }
    _writeCommand(0x13);
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
        _writeData(color_value);
    }
    _Update_Part();
}

void GxEPD2_266_SESc::writeScreenBuffer(uint8_t value)
{
    writeScreenBuffer(value, 0xFF);
}

void GxEPD2_266_SESc::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
    _initial_write = false;  // initial full screen buffer clean done
    _Init_Part();
    _setPartialRamArea(0, 0, WIDTH, HEIGHT);
    _writeCommand(0x10);
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
        _writeData(black_value);
    }
    _writeCommand(0x13);
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
        _writeData(color_value);
    }
}

void GxEPD2_266_SESc::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    writeImage(bitmap, NULL, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_266_SESc::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    if (_initial_write)
        writeScreenBuffer();                                          // initial full screen buffer clean
    delay(1);                                                         // yield() to avoid WDT on ESP8266 and ESP32
    int16_t wb  = (w + 7) / 8;                                        // width bytes, bitmaps are padded
    x          -= x % 8;                                              // byte boundary
    w           = wb * 8;                                             // byte boundary
    int16_t x1  = x < 0 ? 0 : x;                                      // limit
    int16_t y1  = y < 0 ? 0 : y;                                      // limit
    int16_t w1  = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x;    // limit
    int16_t h1  = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y;  // limit
    int16_t dx  = x1 - x;
    int16_t dy  = y1 - y;
    w1         -= dx;
    h1         -= dy;
    if ((w1 <= 0) || (h1 <= 0))
        return;
    _Init_Part();
    _setPartialRamArea(x1, y1, w1, h1);
    _writeCommand(0x10);
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
                if (invert)
                    data = ~data;
            }
            _writeData(data);
        }
    }
    _writeCommand(0x13);
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
                if (invert)
                    data = ~data;
            }
            _writeData(data);
        }
    }
    delay(1);  // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_266_SESc::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    writeImagePart(bitmap, NULL, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_266_SESc::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                     int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    if (_initial_write)
        writeScreenBuffer();  // initial full screen buffer clean
    delay(1);                 // yield() to avoid WDT on ESP8266 and ESP32
    if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0))
        return;
    if ((x_part < 0) || (x_part >= w_bitmap))
        return;
    if ((y_part < 0) || (y_part >= h_bitmap))
        return;
    int16_t wb_bitmap  = (w_bitmap + 7) / 8;                                 // width bytes, bitmaps are padded
    x_part            -= x_part % 8;                                         // byte boundary
    w                  = w_bitmap - x_part < w ? w_bitmap - x_part : w;      // limit
    h                  = h_bitmap - y_part < h ? h_bitmap - y_part : h;      // limit
    x                 -= x % 8;                                              // byte boundary
    w                  = 8 * ((w + 7) / 8);                                  // byte boundary, bitmaps are padded
    int16_t x1         = x < 0 ? 0 : x;                                      // limit
    int16_t y1         = y < 0 ? 0 : y;                                      // limit
    int16_t w1         = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x;    // limit
    int16_t h1         = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y;  // limit
    int16_t dx         = x1 - x;
    int16_t dy         = y1 - y;
    w1                -= dx;
    h1                -= dy;
    if ((w1 <= 0) || (h1 <= 0))
        return;
    if (!_using_partial_mode)
        _Init_Part();
    _setPartialRamArea(x1, y1, w1, h1);
    _writeCommand(0x10);
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
            if (invert)
                data = ~data;
            _writeData(data);
        }
    }
    _writeCommand(0x13);
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
                if (invert)
                    data = ~data;
            }
            _writeData(data);
        }
    }
    delay(1);  // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_266_SESc::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    if (data1)
    {
        writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
    }
}

void GxEPD2_266_SESc::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
    refresh(x, y, w, h);
}

void GxEPD2_266_SESc::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    refresh(x, y, w, h);
}

void GxEPD2_266_SESc::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
    refresh(x, y, w, h);
}

void GxEPD2_266_SESc::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                    int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    refresh(x, y, w, h);
}

void GxEPD2_266_SESc::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
    writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
    refresh(x, y, w, h);
}

void GxEPD2_266_SESc::refresh(bool partial_update_mode)
{
    if (partial_update_mode)
        refresh(0, 0, WIDTH, HEIGHT);
    else
        _Update_Full();
}

void GxEPD2_266_SESc::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
    // intersection with screen
    int16_t w1 = x < 0 ? w + x : w;                                      // reduce
    int16_t h1 = y < 0 ? h + y : h;                                      // reduce
    int16_t x1 = x < 0 ? 0 : x;                                          // limit
    int16_t y1 = y < 0 ? 0 : y;                                          // limit
    w1         = x1 + w1 < int16_t(WIDTH) ? w1 : int16_t(WIDTH) - x1;    // limit
    h1         = y1 + h1 < int16_t(HEIGHT) ? h1 : int16_t(HEIGHT) - y1;  // limit
    if ((w1 <= 0) || (h1 <= 0))
        return;
    // make x1, w1 multiple of 8
    w1 += x1 % 8;
    if (w1 % 8 > 0)
        w1 += 8 - w1 % 8;
    x1 -= x1 % 8;
    _Init_Part();
    _setPartialRamArea(x1, y1, w1, h1);
    _Update_Part();
}

void GxEPD2_266_SESc::powerOff()
{
    _PowerOff();
}

void GxEPD2_266_SESc::hibernate()
{
    _PowerOff();
    delay(10);
    _lcd_chkstatus("hibernate");
    if (_rst >= 0)
    {
        _writeCommand(0x07);  // deep sleep mode
        _writeData(0xa5);     // enter deep sleep
        _hibernating = true;
    }
}

void GxEPD2_266_SESc::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t xe  = (x + w - 1) | 0x0007;  // byte boundary inclusive (last byte)
    uint16_t ye  = y + h - 1;
    x           &= 0xFFF8;                // byte boundary
    _writeCommand(0x91);                  // partial window
    _writeCommand(0x90);                  // partial window
    _writeData(x % 256);
    _writeData(xe % 256 - 1);
    _writeData(y / 256);
    _writeData(y % 256);
    _writeData(ye / 256);
    _writeData(ye % 256 - 1);
    _writeData(0x01);
}

void GxEPD2_266_SESc::_PowerOn()
{
    if (!_power_is_on)
    {
        _writeCommand(0x04);
        _lcd_chkstatus("Power On");
    }
    _power_is_on = true;
}

void GxEPD2_266_SESc::_PowerOff()
{
    if (_power_is_on)
    {
        _writeCommand(0x02);
        _lcd_chkstatus("Power Off");
    }
    _power_is_on = false;
}

void GxEPD2_266_SESc::_InitDisplay()
{
    if (_hibernating)
    {
        _reset();
    }
    delay(10);            // 10ms according to specs
    _PowerOn();           // Power On
    delay(10);            // 10ms according to specs

    _writeCommand(0x00);  // panel setting
    _writeData(0x0f);     // LUT from OTP
    _writeData(0x89);

    _writeCommand(0x01);  // Power Setting
    _writeData(0x03);     // Both Internal
    _writeData(0x00);     // VCOMH=VDH+VCOMDC, VCOML=VHL+VCOMDC //Both VGL VGH 16
    _writeData(0x21);     // 9V
    _writeData(0x21);     // 9V
    _writeData(0x01);     // VDHR 2.6V


    _writeCommand(0x41);  // Enable temperature Sensor
    _writeData(0x07);     // Offset 7

    _writeCommand(0x61);  // resolution setting
    _writeData(0x98);     // 152
    _writeData(0x01);     // 296/256
    _writeData(0x28);     // 296%256

    _writeCommand(0X50);  // VCOM AND DATA INTERVAL SETTING
    _writeData(0x77);     //(0x77);    //WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

    // _setPartialRamArea(0, 0, WIDTH, HEIGHT);
}

void GxEPD2_266_SESc::_Init_Full()
{
    _InitDisplay();
    _PowerOn();
}

void GxEPD2_266_SESc::_Init_Part()
{
    _InitDisplay();
    _PowerOn();
}

void GxEPD2_266_SESc::_Update_Full()
{
    _writeCommand(0x12);
    delay(10);
    _lcd_chkstatus("Update Full");
}

void GxEPD2_266_SESc::_Update_Part()
{
    _writeCommand(0x12);
    delay(10);
    _lcd_chkstatus("Update Part");
}


void GxEPD2_266_SESc::setBorder(uint8_t color)
{
    _writeCommand(0x50);  // VCOM AND DATA INTERVAL SETTING
                          // WBmode:VBDF 17|D7 VBDW 97 VBDB 57
                          // WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7;
    _writeData(color);
}


void GxEPD2_266_SESc::_lcd_chkstatus(String comment)
{
    while (1)
    {
        // Serial.print(".");
        if (isEPD_W21_BUSY == 1)
        {
            Serial.println("e-Paper busy Released");
            break;
        }
        yield();  // Feed dog to prevent system reset
    }
}