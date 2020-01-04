// GxEPD2_Spiffs_Example : Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// BMP handling code extracts taken from: https://github.com/prenticedavid/MCUFRIEND_kbv/tree/master/examples/showBMP_kbv_Uno
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Display ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// mapping suggestion from Waveshare SPI e-Paper to generic ESP8266
// BUSY -> GPIO4, RST -> GPIO2, DC -> GPIO0, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V

// mapping of Waveshare e-Paper ESP8266 Driver Board
// BUSY -> GPIO16, RST -> GPIO5, DC -> GPIO4, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// new mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
// BUSY -> A1, RST -> A2, DC -> A3, CS-> A4, CLK -> A5, DIN -> A7

// mapping suggestion for AVR, UNO, NANO etc.
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 13, DIN -> 11
//

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#if defined(ESP32)
#include "SPIFFS.h"
#endif

#include <FS.h>
#define FileClass fs::File
#define EPD_CS SS

#if defined (ESP8266)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// can use only half buffer size
//GxEPD2_BW < GxEPD2_583, GxEPD2_583::HEIGHT / 2 > display(GxEPD2_583(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_BW < GxEPD2_750, GxEPD2_750::HEIGHT / 2 > display(GxEPD2_750(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// can use only half buffer size
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT / 2> display(GxEPD2_420c(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// can use only quarter buffer size
//GxEPD2_3C<GxEPD2_583c, GxEPD2_583c::HEIGHT / 4> display(GxEPD2_583c(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT / 4> display(GxEPD2_750c(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));

// ***** for mapping of Waveshare e-Paper ESP8266 Driver Board *****
// select one , can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
// can use only half buffer size
//GxEPD2_BW < GxEPD2_583, GxEPD2_583::HEIGHT / 2 > display(GxEPD2_583(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_BW < GxEPD2_750, GxEPD2_750::HEIGHT / 2 > display(GxEPD2_750(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
// can use only half buffer size
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT / 2> display(GxEPD2_420c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
// can use only quarter buffer size
//GxEPD2_3C<GxEPD2_583c, GxEPD2_583c::HEIGHT / 4> display(GxEPD2_583c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT / 4> display(GxEPD2_750c(/*CS=15*/ SS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
#endif

#if defined(ESP32)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
//GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEP015OC1 no longer available
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0154D67
//GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT> display(GxEPD2_213(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
// 3-color e-papers
//GxEPD2_3C<GxEPD2_154c, GxEPD2_154c::HEIGHT> display(GxEPD2_154c(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
//GxEPD2_3C<GxEPD2_750c, GxEPD2_750c::HEIGHT> display(GxEPD2_750c(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
#endif

// function declaration with default parameter
void drawBitmapFromSpiffs(const char *filename, int16_t x, int16_t y, bool with_color = true);

// bitmap drawing using buffered graphics, e.g. for small bitmaps or for GxEPD2_154c
// partial_update selects refresh mode (not effective for GxEPD2_154c)
// overwrite = true does not clear buffer before drawing, use only if buffer is full height
void drawBitmapFromSpiffs_Buffered(const char *filename, int16_t x, int16_t y, bool with_color = true, bool partial_update = false, bool overwrite = false);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD2_Spiffs_Example");

  display.init();

  SPIFFS.begin();
  Serial.println("SPIFFS started");
  listFiles();

  if ((display.epd2.panel == GxEPD2::GDEW0154Z04) || false)
  {
    drawBitmapsBuffered_200x200();
    drawBitmapsBuffered_other();
  }
  else
  {
    drawBitmaps_200x200();
    drawBitmaps_other();
    //drawBitmaps_test();
  }

  Serial.println("GxEPD2_Spiffs_Example done");
}

void loop(void)
{
}

void drawBitmaps_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  drawBitmapFromSpiffs("logo200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("first200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("second200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("third200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("fourth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("fifth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("sixth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("seventh200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs("eighth200x200.bmp", x, y);
  delay(2000);
}

void drawBitmaps_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  drawBitmapFromSpiffs("chanceflurries.bmp", w2 - 50, h2 - 50, false);
  delay(2000);
  drawBitmapFromSpiffs("betty_1.bmp", w2 - 100, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs("betty_4.bmp", w2 - 102, h2 - 126);
  delay(2000);
  drawBitmapFromSpiffs("marilyn_240x240x8.bmp", w2 - 120, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs("miniwoof.bmp", w2 - 60, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs("tiger.bmp", w2 - 160, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs("tiger_178x160x4.bmp", w2 - 89, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs("tiger_240x317x4.bmp", w2 - 120, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs("tiger_320x200x24.bmp", w2 - 160, h2 - 100);
  delay(2000);
  drawBitmapFromSpiffs("woof.bmp", w2 - 120, h2 - 160);
  delay(2000);
}

void drawBitmaps_test()
{
  drawBitmapFromSpiffs("output5.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("output6.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_1.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_4.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_8.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_11.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_44.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSpiffs("tractor_88.bmp", 0, 0);
  delay(2000);
}

void drawBitmapsBuffered_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  drawBitmapFromSpiffs_Buffered("logo200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("first200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("second200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("third200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("fourth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("fifth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("sixth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("seventh200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("eighth200x200.bmp", x, y);
  delay(2000);
}

void drawBitmapsBuffered_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  drawBitmapFromSpiffs_Buffered("chanceflurries.bmp", w2 - 50, h2 - 50, false);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("betty_1.bmp", w2 - 100, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("betty_4.bmp", w2 - 102, h2 - 126);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("marilyn_240x240x8.bmp", w2 - 120, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("miniwoof.bmp", w2 - 60, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("tiger.bmp", w2 - 160, h2 - 120);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("tiger_178x160x4.bmp", w2 - 89, h2 - 80);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("tiger_240x317x4.bmp", w2 - 120, h2 - 160);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("tiger_320x200x24.bmp", w2 - 160, h2 - 100);
  delay(2000);
  drawBitmapFromSpiffs_Buffered("woof.bmp", w2 - 120, h2 - 160);
  delay(2000);
}

static const uint16_t input_buffer_pixels = 800; // may affect performance

static const uint16_t max_row_width = 800; // for up to 7.5" display 800x480
static const uint16_t max_palette_pixels = 256; // for depth <= 8

uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t output_row_color_buffer[max_row_width / 8]; // buffer for at least one row of color bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t color_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 c/w

void drawBitmapFromSpiffs(const char *filename, int16_t x, int16_t y, bool with_color)
{
  fs::File file;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.width()) || (y >= display.height())) return;
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
#if defined(ESP32)
  file = SPIFFS.open(String("/") + filename, "r");
#else
  file = SPIFFS.open(filename, "r");
#endif
  if (!file)
  {
    Serial.print("File not found");
    return;
  }
  // Parse BMP header
  if (read16(file) == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(file);
    uint32_t creatorBytes = read32(file);
    uint32_t imageOffset = read32(file); // Start of image data
    uint32_t headerSize = read32(file);
    uint32_t width  = read32(file);
    uint32_t height = read32(file);
    uint16_t planes = read16(file);
    uint16_t depth = read16(file); // bits per pixel
    uint32_t format = read32(file);
    if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
    {
      Serial.print("File size: "); Serial.println(fileSize);
      Serial.print("Image Offset: "); Serial.println(imageOffset);
      Serial.print("Header size: "); Serial.println(headerSize);
      Serial.print("Bit Depth: "); Serial.println(depth);
      Serial.print("Image size: ");
      Serial.print(width);
      Serial.print('x');
      Serial.println(height);
      // BMP rows are padded (if needed) to 4-byte boundary
      uint32_t rowSize = (width * depth / 8 + 3) & ~3;
      if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
      if (height < 0)
      {
        height = -height;
        flip = false;
      }
      uint16_t w = width;
      uint16_t h = height;
      if ((x + w - 1) >= display.width())  w = display.width()  - x;
      if ((y + h - 1) >= display.height()) h = display.height() - y;
      if (w <= max_row_width) // handle with direct drawing
      {
        valid = true;
        uint8_t bitmask = 0xFF;
        uint8_t bitshift = 8 - depth;
        uint16_t red, green, blue;
        bool whitish, colored;
        if (depth == 1) with_color = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //file.seek(54); //palette is always @ 54
          file.seek(imageOffset - (4 << depth)); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = file.read();
            green = file.read();
            red   = file.read();
            file.read();
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
          }
        }
        display.clearScreen();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint8_t out_color_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          file.seek(rowPosition);
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              in_bytes = file.read(input_buffer, in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain);
              in_remain -= in_bytes;
              in_idx = 0;
            }
            switch (depth)
            {
              case 24:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                break;
              case 16:
                {
                  uint8_t lsb = input_buffer[in_idx++];
                  uint8_t msb = input_buffer[in_idx++];
                  if (format == 0) // 555
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                    red   = (msb & 0x7C) << 1;
                  }
                  else // 565
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                    red   = (msb & 0xF8);
                  }
                  whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                  colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                }
                break;
              case 1:
              case 4:
              case 8:
                {
                  if (0 == in_bits)
                  {
                    in_byte = input_buffer[in_idx++];
                    in_bits = 8;
                  }
                  uint16_t pn = (in_byte >> bitshift) & bitmask;
                  whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  in_byte <<= depth;
                  in_bits -= depth;
                }
                break;
            }
            if (whitish)
            {
              // keep white
            }
            else if (colored && with_color)
            {
              out_color_byte &= ~(0x80 >> col % 8); // colored
            }
            else
            {
              out_byte &= ~(0x80 >> col % 8); // black
            }
            if ((7 == col % 8) || (col == w - 1)) // write that last byte! (for w%8!=0 border)
            {
              output_row_color_buffer[out_idx] = out_color_byte;
              output_row_mono_buffer[out_idx++] = out_byte;
              out_byte = 0xFF; // white (for w%8!=0 border)
              out_color_byte = 0xFF; // white (for w%8!=0 border)
            }
          } // end pixel
          uint16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_mono_buffer, output_row_color_buffer, x, yrow, w, 1);
        } // end line
        Serial.print("loaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
        display.refresh();
      }
    }
  }
  file.close();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void drawBitmapFromSpiffs_Buffered(const char *filename, int16_t x, int16_t y, bool with_color, bool partial_update, bool overwrite)
{
  fs::File file;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.width()) || (y >= display.height())) return;
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
#if defined(ESP32)
  file = SPIFFS.open(String("/") + filename, "r");
#else
  file = SPIFFS.open(filename, "r");
#endif
  if (!file)
  {
    Serial.print("File not found");
    return;
  }
  // Parse BMP header
  if (read16(file) == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(file);
    uint32_t creatorBytes = read32(file);
    uint32_t imageOffset = read32(file); // Start of image data
    uint32_t headerSize = read32(file);
    uint32_t width  = read32(file);
    uint32_t height = read32(file);
    uint16_t planes = read16(file);
    uint16_t depth = read16(file); // bits per pixel
    uint32_t format = read32(file);
    if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
    {
      Serial.print("File size: "); Serial.println(fileSize);
      Serial.print("Image Offset: "); Serial.println(imageOffset);
      Serial.print("Header size: "); Serial.println(headerSize);
      Serial.print("Bit Depth: "); Serial.println(depth);
      Serial.print("Image size: ");
      Serial.print(width);
      Serial.print('x');
      Serial.println(height);
      // BMP rows are padded (if needed) to 4-byte boundary
      uint32_t rowSize = (width * depth / 8 + 3) & ~3;
      if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
      if (height < 0)
      {
        height = -height;
        flip = false;
      }
      uint16_t w = width;
      uint16_t h = height;
      if ((x + w - 1) >= display.width())  w = display.width()  - x;
      if ((y + h - 1) >= display.height()) h = display.height() - y;
      //if (w <= max_row_width) // handle with direct drawing
      {
        valid = true;
        uint8_t bitmask = 0xFF;
        uint8_t bitshift = 8 - depth;
        uint16_t red, green, blue;
        bool whitish, colored;
        if (depth == 1) with_color = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //file.seek(54); //palette is always @ 54
          file.seek(imageOffset - (4 << depth)); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = file.read();
            green = file.read();
            red   = file.read();
            file.read();
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
          }
        }
        if (partial_update) display.setPartialWindow(x, y, w, h);
        else display.setFullWindow();
        display.firstPage();
        do
        {
          if (!overwrite) display.fillScreen(GxEPD_WHITE);
          uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
          for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
          {
            uint32_t in_remain = rowSize;
            uint32_t in_idx = 0;
            uint32_t in_bytes = 0;
            uint8_t in_byte = 0; // for depth <= 8
            uint8_t in_bits = 0; // for depth <= 8
            uint16_t color = GxEPD_WHITE;
            file.seek(rowPosition);
            for (uint16_t col = 0; col < w; col++) // for each pixel
            {
              // Time to read more pixel data?
              if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
              {
                in_bytes = file.read(input_buffer, in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain);
                in_remain -= in_bytes;
                in_idx = 0;
              }
              switch (depth)
              {
                case 24:
                  blue = input_buffer[in_idx++];
                  green = input_buffer[in_idx++];
                  red = input_buffer[in_idx++];
                  whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                  colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                  break;
                case 16:
                  {
                    uint8_t lsb = input_buffer[in_idx++];
                    uint8_t msb = input_buffer[in_idx++];
                    if (format == 0) // 555
                    {
                      blue  = (lsb & 0x1F) << 3;
                      green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                      red   = (msb & 0x7C) << 1;
                    }
                    else // 565
                    {
                      blue  = (lsb & 0x1F) << 3;
                      green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                      red   = (msb & 0xF8);
                    }
                    whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                    colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                  }
                  break;
                case 1:
                case 4:
                case 8:
                  {
                    if (0 == in_bits)
                    {
                      in_byte = input_buffer[in_idx++];
                      in_bits = 8;
                    }
                    uint16_t pn = (in_byte >> bitshift) & bitmask;
                    whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                    colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                    in_byte <<= depth;
                    in_bits -= depth;
                  }
                  break;
              }
              if (whitish)
              {
                color = GxEPD_WHITE;
              }
              else if (colored && with_color)
              {
                color = GxEPD_COLORED;
              }
              else
              {
                color = GxEPD_BLACK;
              }
              uint16_t yrow = y + (flip ? h - row - 1 : row);
              display.drawPixel(x + col, yrow, color);
            } // end pixel
          } // end line
        }
        while (display.nextPage());
        Serial.print("loaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
      }
    }
  }
  file.close();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

uint16_t read16(fs::File& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
