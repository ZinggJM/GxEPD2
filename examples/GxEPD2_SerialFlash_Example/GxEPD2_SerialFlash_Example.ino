// GxEPD2_SerialFlash_Example : Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
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
//
// this example uses the SerialFlash library from: https://github.com/PaulStoffregen/SerialFlash
// with a modification for use with ESP32 or the STM32 package available here: https://github.com/ZinggJM/SerialFlash
// download it as .zip file and install with Library Mananger method "Add .ZIP Library..."
//
// note that BMP bitmaps are drawn at physical position in physical orientation of the screen

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Display ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

// see GxEPD2_wiring_examples.h for wiring suggestions and examples

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>
// Note: if you use this with ENABLE_GxEPD2_GFX 1:
//       uncomment it in GxEPD2_GFX.h too, or add #include <GFX.h> before any #include <GxEPD2_GFX.h>

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include <SerialFlash.h>

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"

#if defined (ESP8266)
const int FlashChipSelect = D1; // digital pin for flash chip CS pin
#endif

#if defined(ESP32)
//const int FlashChipSelect = 17; // as used with my ESP32 breadboard
const int FlashChipSelect = 2; // as used with my ESP32 protoboard SD connector
#endif

// function declaration with default parameter
// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void drawBitmapFromSerialFlash(const char *filename, int16_t x, int16_t y, bool with_color = true);

// bitmap drawing using buffered graphics, e.g. for small bitmaps or for GxEPD2_154c
// draws BMP bitmap according to set orientation
// partial_update selects refresh mode (not effective for GxEPD2_154c)
// overwrite = true does not clear buffer before drawing, use only if buffer is full height
void drawBitmapFromSerialFlash_Buffered(const char *filename, int16_t x, int16_t y, bool with_color = true, bool partial_update = false, bool overwrite = false);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD2_SerialFlash_Example");

  display.init();

  if (!SerialFlash.begin(FlashChipSelect))
  {
    Serial.println("Unable to access SPI Flash chip");
    return;
  }
  Serial.println("SerialFlash started");

  listFiles();

  if ((display.epd2.panel == GxEPD2::GDEW0154Z04) || (display.epd2.panel == GxEPD2::ACeP565) || false)
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

  Serial.println("GxEPD2_SerialFlash_Example done");
}

void loop(void)
{
}

void listFiles()
{
  Serial.println("All Files on SPI Flash chip:");
  SerialFlash.opendir();
  while (1)
  {
    char filename[64];
    uint32_t filesize;

    if (SerialFlash.readdir(filename, sizeof(filename), filesize))
    {
      Serial.print("  ");
      Serial.print(filename);
      spaces(20 - strlen(filename));
      Serial.print("  ");
      Serial.print(filesize);
      Serial.print(" bytes");
      Serial.println();
    }
    else
    {
      Serial.println("no more files...");
      break; // no more files
    }
  }
}

void spaces(int num)
{
  for (int i = 0; i < num; i++)
  {
    Serial.print(" ");
  }
}

void drawBitmaps_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  drawBitmapFromSerialFlash("logo200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("first200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("second200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("third200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("fourth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("fifth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("sixth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("seventh200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash("eighth200x200.bmp", x, y);
  delay(2000);
}

void drawBitmaps_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  drawBitmapFromSerialFlash("chanceflurries.bmp", w2 - 50, h2 - 50, false);
  delay(2000);
  drawBitmapFromSerialFlash("betty_1.bmp", w2 - 100, h2 - 160);
  delay(2000);
  drawBitmapFromSerialFlash("betty_4.bmp", w2 - 102, h2 - 126);
  delay(2000);
  drawBitmapFromSerialFlash("marilyn_240x240x8.bmp", w2 - 120, h2 - 120);
  delay(2000);
  drawBitmapFromSerialFlash("miniwoof.bmp", w2 - 60, h2 - 80);
  delay(2000);
  drawBitmapFromSerialFlash("tiger.bmp", w2 - 160, h2 - 120);
  delay(2000);
  drawBitmapFromSerialFlash("tiger_178x160x4.bmp", w2 - 89, h2 - 80);
  delay(2000);
  drawBitmapFromSerialFlash("tiger_240x317x4.bmp", w2 - 120, h2 - 160);
  delay(2000);
  drawBitmapFromSerialFlash("tiger_320x200x24.bmp", w2 - 160, h2 - 100);
  delay(2000);
  drawBitmapFromSerialFlash("woof.bmp", w2 - 120, h2 - 160);
  delay(2000);
}

void drawBitmaps_test()
{
  drawBitmapFromSerialFlash("output5.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSerialFlash("output6.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSerialFlash("tractor_1.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSerialFlash("tractor_4.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSerialFlash("tractor_8.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSerialFlash("tractor_11.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSerialFlash("tractor_44.bmp", 0, 0);
  delay(2000);
  drawBitmapFromSerialFlash("tractor_88.bmp", 0, 0);
  delay(2000);
}

void drawBitmapsBuffered_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  drawBitmapFromSerialFlash_Buffered("logo200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("first200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("second200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("third200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("fourth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("fifth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("sixth200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("seventh200x200.bmp", x, y);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("eighth200x200.bmp", x, y);
  delay(2000);
}

void drawBitmapsBuffered_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  drawBitmapFromSerialFlash_Buffered("chanceflurries.bmp", w2 - 50, h2 - 50, false);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("betty_1.bmp", w2 - 100, h2 - 160);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("betty_4.bmp", w2 - 102, h2 - 126);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("marilyn_240x240x8.bmp", w2 - 120, h2 - 120);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("miniwoof.bmp", w2 - 60, h2 - 80);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("tiger.bmp", w2 - 160, h2 - 120);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("tiger_178x160x4.bmp", w2 - 89, h2 - 80);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("tiger_240x317x4.bmp", w2 - 120, h2 - 160);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("tiger_320x200x24.bmp", w2 - 160, h2 - 100);
  delay(2000);
  drawBitmapFromSerialFlash_Buffered("woof.bmp", w2 - 120, h2 - 160);
  delay(2000);
}

static const uint16_t input_buffer_pixels = 800; // may affect performance

static const uint16_t max_row_width = 1448; // for up to 6" display 1448x1072
static const uint16_t max_palette_pixels = 256; // for depth <= 8

uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t output_row_color_buffer[max_row_width / 8]; // buffer for at least one row of color bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t color_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 c/w
uint16_t rgb_palette_buffer[max_palette_pixels]; // palette buffer for depth <= 8 for buffered graphics, needed for 7-color display

void drawBitmapFromSerialFlash(const char *filename, int16_t x, int16_t y, bool with_color)
{
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd2.WIDTH) || (y >= display.epd2.HEIGHT)) return;
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
  SerialFlashFile file = SerialFlash.open(filename);
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
      if ((x + w - 1) >= display.epd2.WIDTH)  w = display.epd2.WIDTH  - x;
      if ((y + h - 1) >= display.epd2.HEIGHT) h = display.epd2.HEIGHT - y;
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
            blue  = read8(file);
            green = read8(file);
            red   = read8(file);
            read8(file);
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

void drawBitmapFromSerialFlash_Buffered(const char *filename, int16_t x, int16_t y, bool with_color, bool partial_update, bool overwrite)
{
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  bool has_multicolors = display.epd2.panel == GxEPD2::ACeP565;
  uint32_t startTime = millis();
  if ((x >= display.width()) || (y >= display.height())) return;
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
  SerialFlashFile file = SerialFlash.open(filename);
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
            blue  = read8(file);
            green = read8(file);
            red   = read8(file);
            read8(file);
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
            rgb_palette_buffer[pn] = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
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
                  color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
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
                      color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
                    }
                    else // 565
                    {
                      blue  = (lsb & 0x1F) << 3;
                      green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                      red   = (msb & 0xF8);
                      color = (msb << 8) | lsb;
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
                    color = rgb_palette_buffer[pn];
                  }
                  break;
              }
              if (with_color && has_multicolors)
              {
                // keep color
              }
              else if (whitish)
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

uint8_t read8(SerialFlashFile& f)
{
  uint8_t result;
  f.read((void*)&result, 1);
  return result;
}

uint16_t read16(SerialFlashFile& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  f.read((void*)&result, 2);
  return result;
}

uint32_t read32(SerialFlashFile& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  f.read((void*)&result, 4);
  return result;
}
