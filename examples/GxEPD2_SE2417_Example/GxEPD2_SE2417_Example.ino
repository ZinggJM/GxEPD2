// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Example for SE2417FS051 4.2" 3-color (Black-White-Red) e-paper display
// Display: SE2417FS051 400x300 pixels, IL0398-compatible controller
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// IMPORTANT: The e-paper panels require 3.3V supply AND 3.3V data lines!
// Never connect data lines directly to 5V Arduino pins!
// Use 4k7/10k resistor dividers or level converters for 5V boards

#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

// Select your display and pin configuration here
// For SE2417FS051 4.2" 3-color display

#if defined(ESP32)
// Default pinout (standard for SE2417FS051 boards):
// VCC  -> 3.3V (Power positive - 3.3V power supply input)
// GND  -> GND  (Ground)
// DIN  -> P14  (GPIO 14, HSPI MOSI - data input)
// SCLK -> P13  (GPIO 13, HSPI SCK - clock signal input)
// CS   -> P15  (GPIO 15, Chip selection, low active)
// DC   -> P27  (GPIO 27, Data/Command, low for command, high for data)
// RST  -> P26  (GPIO 26, Reset, low active)
// BUSY -> P25  (GPIO 25, Busy status output pin)
GxEPD2_3C<GxEPD2_420c_SE2417, GxEPD2_420c_SE2417::HEIGHT> display(GxEPD2_420c_SE2417(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
// NOTE: This uses HSPI with remapped pins (see SPI.begin() in setup())

// Alternative pinout for generic ESP32 boards with standard VSPI:
// GxEPD2_3C<GxEPD2_420c_SE2417, GxEPD2_420c_SE2417::HEIGHT> display(GxEPD2_420c_SE2417(/*CS=*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
// BUSY -> 4, RST -> 16, DC -> 17, CS -> 5, CLK -> 18 (VSPI), DIN -> 23 (VSPI)

#elif defined(ESP8266)
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7
GxEPD2_3C<GxEPD2_420c_SE2417, GxEPD2_420c_SE2417::HEIGHT> display(GxEPD2_420c_SE2417(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// NOTE: connect 3.3k pull-down from D8 (CS) to GND if your board or shield has level converters
// NOTE: connect 1k pull-up from D4 (RST) to 3.3V if your board has "clever" reset circuit

#else
// BUSY -> 7, RST -> 9, DC -> 8, CS -> 10, CLK -> 13, DIN -> 11
GxEPD2_3C<GxEPD2_420c_SE2417, GxEPD2_420c_SE2417::HEIGHT> display(GxEPD2_420c_SE2417(/*CS=*/ 10, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
#endif

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD2 SE2417FS051 Example");

#if defined(ESP32)
  // Remap SPI pins for HSPI configuration (P13=SCK, P14=MOSI, P12=MISO, P15=CS)
  // Required for the default SE2417FS051 pinout
  SPI.end();
  SPI.begin(13, 12, 14, 15); // SCK, MISO, MOSI, SS
  Serial.println("Using HSPI: SCK=13, MOSI=14, MISO=12, CS=15");
#endif

  display.init(115200); // Initialize display with diagnostic output at 115200 baud

  // Run through some basic drawing examples
  helloWorld();
  delay(3000);

  helloFullScreenPartialMode();
  delay(3000);

  helloArduino();
  delay(3000);

  showColors();
  delay(3000);

  showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
  delay(3000);

  showFont("FreeMonoBold12pt7b", &FreeMonoBold12pt7b);
  delay(3000);

  drawGraphics();
  delay(3000);

  Serial.println("Setup complete");
}

void loop()
{
  // Nothing to do here - all examples run in setup()
}

void helloWorld()
{
  Serial.println("helloWorld");
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 20);
    display.println("Hello World!");
  }
  while (display.nextPage());
  Serial.println("helloWorld done");
}

void helloFullScreenPartialMode()
{
  Serial.println("helloFullScreenPartialMode");
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 20);
    display.println("Hello World!");
    display.setCursor(10, 40);
    display.println("Partial Mode");
  }
  while (display.nextPage());
  Serial.println("helloFullScreenPartialMode done");
}

void helloArduino()
{
  Serial.println("helloArduino");
  display.setRotation(0);
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(50, 60);
    display.println("Hello Arduino!");
    display.setCursor(50, 90);
    display.println("SE2417FS051");
    display.setCursor(50, 120);
    display.println("4.2\" 3-color");
    display.setCursor(50, 150);
    display.println("400x300 pixels");
  }
  while (display.nextPage());
  Serial.println("helloArduino done");
}

void showColors()
{
  Serial.println("showColors");
  display.setRotation(0);
  display.setFont(&FreeMonoBold18pt7b);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    // Draw black text
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(10, 40);
    display.println("BLACK TEXT");

    // Draw red text
    display.setTextColor(GxEPD_RED);
    display.setCursor(10, 80);
    display.println("RED TEXT");

    // Draw black rectangle
    display.fillRect(10, 100, 100, 50, GxEPD_BLACK);

    // Draw red rectangle
    display.fillRect(120, 100, 100, 50, GxEPD_RED);

    // Draw white rectangle with black border
    display.fillRect(230, 100, 100, 50, GxEPD_WHITE);
    display.drawRect(230, 100, 100, 50, GxEPD_BLACK);

    // Draw mixed color circles
    display.fillCircle(60, 220, 30, GxEPD_BLACK);
    display.fillCircle(170, 220, 30, GxEPD_RED);
    display.drawCircle(280, 220, 30, GxEPD_BLACK);
  }
  while (display.nextPage());
  Serial.println("showColors done");
}

void showFont(const char name[], const GFXfont* f)
{
  Serial.print("showFont ");
  Serial.println(name);
  display.setRotation(0);
  display.setFont(f);
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 20);
    display.println(name);
    display.setCursor(10, 50);
    display.println("ABCDEFGHIJKLM");
    display.setCursor(10, 80);
    display.println("NOPQRSTUVWXYZ");
    display.setCursor(10, 110);
    display.println("abcdefghijklm");
    display.setCursor(10, 140);
    display.println("nopqrstuvwxyz");
    display.setCursor(10, 170);
    display.println("0123456789");
    display.setCursor(10, 200);
    display.println("!@#$%^&*()");
  }
  while (display.nextPage());
  Serial.print("showFont ");
  Serial.print(name);
  Serial.println(" done");
}

void drawGraphics()
{
  Serial.println("drawGraphics");
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    // Title
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(80, 20);
    display.println("Graphics Demo");

    // Lines
    for (int i = 0; i < 5; i++)
    {
      display.drawLine(10 + i * 20, 40, 10 + i * 20, 100, GxEPD_BLACK);
      display.drawLine(110, 40 + i * 12, 200, 40 + i * 12, GxEPD_RED);
    }

    // Rectangles
    display.drawRect(10, 120, 80, 60, GxEPD_BLACK);
    display.fillRect(20, 130, 60, 40, GxEPD_RED);

    // Circles
    display.drawCircle(140, 150, 25, GxEPD_BLACK);
    display.fillCircle(140, 150, 15, GxEPD_RED);

    // Triangles
    display.drawTriangle(210, 120, 240, 180, 180, 180, GxEPD_BLACK);
    display.fillTriangle(220, 130, 230, 170, 190, 170, GxEPD_RED);

    // Rounded rectangles
    display.drawRoundRect(260, 120, 80, 60, 10, GxEPD_BLACK);
    display.fillRoundRect(270, 130, 60, 40, 8, GxEPD_RED);

    // Text at bottom
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(10, 220);
    display.println("SE2417FS051 supports");
    display.setCursor(10, 240);
    display.println("Black, White & Red!");
  }
  while (display.nextPage());
  Serial.println("drawGraphics done");
}
