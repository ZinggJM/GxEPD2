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

#include "GxEPD2_EPD.h"

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

GxEPD2_EPD::GxEPD2_EPD(int8_t cs, int8_t dc, int8_t rst, int8_t busy, int8_t busy_level, uint32_t busy_timeout,
                       uint16_t w, uint16_t h, GxEPD2::Panel p, bool c, bool pu, bool fpu) :
  WIDTH(w), HEIGHT(h), panel(p), hasColor(c), hasPartialUpdate(pu), hasFastPartialUpdate(fpu),
  _cs(cs), _dc(dc), _rst(rst), _busy(busy), _busy_level(busy_level), _busy_timeout(busy_timeout), _diag_enabled(false),
  _spi_settings(4000000, MSBFIRST, SPI_MODE0)
{
}

void GxEPD2_EPD::init(uint32_t serial_diag_bitrate)
{
  if (serial_diag_bitrate > 0)
  {
    Serial.begin(serial_diag_bitrate);
    _diag_enabled = true;
  }
  if (_cs >= 0)
  {
    digitalWrite(_cs, HIGH);
    pinMode(_cs, OUTPUT);
  }
  if (_dc >= 0)
  {
    digitalWrite(_dc, HIGH);
    pinMode(_dc, OUTPUT);
  }
  if (_rst >= 0)
  {
    digitalWrite(_rst, HIGH);
    pinMode(_rst, OUTPUT);
    delay(20);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(200);
  }
  if (_busy >= 0)
  {
    pinMode(_busy, INPUT);
  }
  SPI.begin();
  //  SPI.setDataMode(SPI_MODE0);
  //  SPI.setBitOrder(MSBFIRST);
  //#if defined(SPI_HAS_TRANSACTION)
  //  // true also for STM32F1xx Boards
  //  SPISettings settings(4000000, MSBFIRST, SPI_MODE0);
  //  SPI.beginTransaction(settings);
  //  SPI.endTransaction();
  //  //Serial.println("SPI has Transaction");
  //#elif defined(ESP8266) || defined(ESP32)
  //  SPI.setFrequency(4000000);
  //#endif
}

void GxEPD2_EPD::_waitWhileBusy(const char* comment, uint16_t busy_time)
{
  if (_busy >= 0)
  {
    unsigned long start = micros();
    while (1)
    {
      if (digitalRead(_busy) != _busy_level) break;
      delay(1);
      if (micros() - start > _busy_timeout)
      {
        Serial.println("Busy Timeout!");
        break;
      }
    }
    if (comment)
    {
#if !defined(DISABLE_DIAGNOSTIC_OUTPUT)
      if (_diag_enabled)
      {
        unsigned long elapsed = micros() - start;
        Serial.print(comment);
        Serial.print(" : ");
        Serial.println(elapsed);
      }
#endif
    }
    (void) start;
  }
  else delay(busy_time);
}

void GxEPD2_EPD::_writeCommand(uint8_t c)
{
  SPI.beginTransaction(_spi_settings);
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  SPI.transfer(c);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
  SPI.endTransaction();
}

void GxEPD2_EPD::_writeData(uint8_t d)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  SPI.transfer(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void GxEPD2_EPD::_writeData(const uint8_t* data, uint16_t n)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  for (uint8_t i = 0; i < n; i++)
  {
    SPI.transfer(*data++);
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void GxEPD2_EPD::_writeDataPGM(const uint8_t* data, uint16_t n)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  for (uint8_t i = 0; i < n; i++)
  {
    SPI.transfer(pgm_read_byte(&*data++));
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void GxEPD2_EPD::_writeCommandData(const uint8_t* pCommandData, uint8_t datalen)
{
  SPI.beginTransaction(_spi_settings);
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  SPI.transfer(*pCommandData++);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
  for (uint8_t i = 0; i < datalen - 1; i++)  // sub the command
  {
    SPI.transfer(*pCommandData++);
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void GxEPD2_EPD::_writeCommandDataPGM(const uint8_t* pCommandData, uint8_t datalen)
{
  SPI.beginTransaction(_spi_settings);
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  SPI.transfer(pgm_read_byte(&*pCommandData++));
  if (_dc >= 0) digitalWrite(_dc, HIGH);
  for (uint8_t i = 0; i < datalen - 1; i++)  // sub the command
  {
    SPI.transfer(pgm_read_byte(&*pCommandData++));
  }
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}


