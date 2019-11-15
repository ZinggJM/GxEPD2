// GxEPD2_Spiffs_Loader : Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Display ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

#if defined(ESP32)
#include "SPIFFS.h"
// set formatOnFail = true for initial format of ESP32 SPIFFS (e.g. if error occured)
const bool formatOnFail = false;
#endif

#include <FS.h>

#if defined (ESP8266)
#include <ESP8266WiFi.h>
#define USE_BearSSL true
#endif

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

const char* ssid     = "........";
const char* password = "........";
const int httpPort  = 80;
const int httpsPort = 443;
const char* fp_api_github_com = "35 85 74 EF 67 35 A7 CE 40 69 50 F3 C0 F6 80 CF 80 3B 2E 19";
const char* fp_github_com     = "ca 06 f5 6b 25 8b 7a 0d 4f 2b 05 47 09 39 47 86 51 15 19 84";
#if USE_BearSSL
const char fp_rawcontent[20]  = {0xcc, 0xaa, 0x48, 0x48, 0x66, 0x46, 0x0e, 0x91, 0x53, 0x2c, 0x9c, 0x7c, 0x23, 0x2a, 0xb1, 0x74, 0x4d, 0x29, 0x9d, 0x33};
#else
const char* fp_rawcontent     = "cc aa 48 48 66 46 0e 91 53 2c 9c 7c 23 2a b1 74 4d 29 9d 33";
#endif
const char* host_rawcontent   = "raw.githubusercontent.com";
const char* path_rawcontent   = "/ZinggJM/GxEPD2/master/extras/bitmaps/";
const char* path_prenticedavid   = "/prenticedavid/MCUFRIEND_kbv/master/extras/bitmaps/";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD2_Spiffs_Loader");

#ifdef RE_INIT_NEEDED
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.disconnect();
#endif

  if (!WiFi.getAutoConnect() || ( WiFi.getMode() != WIFI_STA) || ((WiFi.SSID() != ssid) && String(ssid) != "........"))
  {
    Serial.println();
    Serial.print("WiFi.getAutoConnect()=");
    Serial.println(WiFi.getAutoConnect());
    Serial.print("WiFi.SSID()=");
    Serial.println(WiFi.SSID());
    WiFi.mode(WIFI_STA); // switch off AP
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  }
  int ConnectTimeout = 30; // 15 seconds
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
    if (--ConnectTimeout <= 0)
    {
      Serial.println();
      Serial.println("WiFi connect timeout");
      return;
    }
  }
  Serial.println();
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

#if defined(ESP32)
  SPIFFS.begin(formatOnFail);
#else
  SPIFFS.begin();
#endif
  Serial.println("SPIFFS started");
  listFiles();
  //deleteFiles();
  downloadBitmaps_200x200();
  downloadBitmaps_other();
  //downloadBitmaps_test();
  listFiles();
}

void loop()
{
}

void downloadBitmaps_200x200()
{
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "logo200x200.bmp", fp_rawcontent, "logo200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "first200x200.bmp", fp_rawcontent, "first200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "second200x200.bmp", fp_rawcontent, "second200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "third200x200.bmp", fp_rawcontent, "third200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "fourth200x200.bmp", fp_rawcontent, "fourth200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "fifth200x200.bmp", fp_rawcontent, "fifth200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "sixth200x200.bmp", fp_rawcontent, "sixth200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "seventh200x200.bmp", fp_rawcontent, "seventh200x200.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "eighth200x200.bmp", fp_rawcontent, "eighth200x200.bmp");
}

void downloadBitmaps_other()
{
  downloadFile_HTTP("www.squix.org", "/blog/wunderground/", "chanceflurries.bmp", "chanceflurries.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "betty_1.bmp", fp_rawcontent, "betty_1.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, "betty_4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "marilyn_240x240x8.bmp", fp_rawcontent, "marilyn_240x240x8.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "miniwoof.bmp", fp_rawcontent, "miniwoof.bmp");
  //downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "test.bmp", fp_rawcontent, "test.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger.bmp", fp_rawcontent, "tiger.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger_178x160x4.bmp", fp_rawcontent, "tiger_178x160x4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger_240x317x4.bmp", fp_rawcontent, "tiger_240x317x4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger_320x200x24.bmp", fp_rawcontent, "tiger_320x200x24.bmp");
  //downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "tiger16T.bmp", fp_rawcontent, "tiger16T.bmp");
  downloadFile_HTTPS(host_rawcontent, path_prenticedavid, "woof.bmp", fp_rawcontent, "woof.bmp");
}

void downloadBitmaps_test()
{
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "output5.bmp", fp_rawcontent, "output5.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "output6.bmp", fp_rawcontent, "output6.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_1.bmp", fp_rawcontent, "tractor_1.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_4.bmp", fp_rawcontent, "tractor_4.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_8.bmp", fp_rawcontent, "tractor_8.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_11.bmp", fp_rawcontent, "tractor_11.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_44.bmp", fp_rawcontent, "tractor_44.bmp");
  downloadFile_HTTPS(host_rawcontent, path_rawcontent, "tractor_88.bmp", fp_rawcontent, "tractor_88.bmp");
}

void deleteFiles()
{
  SPIFFS.remove("logo200x200.bmp");
  SPIFFS.remove("first200x200.bmp");
  SPIFFS.remove("second200x200.bmp");
  SPIFFS.remove("third200x200.bmp");
  SPIFFS.remove("fourth200x200.bmp");
  SPIFFS.remove("fifth200x200.bmp");
  SPIFFS.remove("sixth200x200.bmp");
  SPIFFS.remove("seventh200x200.bmp");
  SPIFFS.remove("eighth200x200.bmp");
  SPIFFS.remove("chanceflurries.bmp");
  SPIFFS.remove("betty_1.bmp");
  SPIFFS.remove("betty_4.bmp");
  SPIFFS.remove("marilyn_240x240x8.bmp");
  SPIFFS.remove("miniwoof.bmp");
  SPIFFS.remove("test.bmp");
  SPIFFS.remove("tiger.bmp");
  SPIFFS.remove("tiger_178x160x4.bmp");
  SPIFFS.remove("tiger_240x317x4.bmp");
  SPIFFS.remove("tiger_320x200x24.bmp");
  SPIFFS.remove("tiger16T.bmp");
  SPIFFS.remove("woof.bmp");
}

void downloadFile_HTTP(const char* host, const char* path, const char* filename, const char* target)
{
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("http://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_Spiffs_Loader\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  bool ok = false;
  while (client.connected() || client.available())
  {
    String line = client.readStringUntil('\n');
    if (!ok)
    {
      ok = line.startsWith("HTTP/1.1 200 OK");
      if (ok) Serial.println(line);
      //if (!ok) Serial.println(line);
    }
    if (!ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!ok) return;
  uint8_t buffer[512];
#if defined(ESP8266)
  client.peekBytes(buffer, 2);
  Serial.write(buffer[0]); Serial.write(buffer[1]); Serial.println();
#endif
  size_t total = 0;
#if defined(ESP32)
  fs::File file = SPIFFS.open(String("/") + target, "w+");
#else
  fs::File file = SPIFFS.open(target, "w+");
#endif
  if (!file)
  {
    Serial.print(target); Serial.println(" open failed");
    return;
  }
  while (client.connected() || client.available())
  {
    size_t available = client.available();
    size_t fetch = available <= sizeof(buffer) ? available : sizeof(buffer);
    if (fetch > 0)
    {
      size_t got = client.read(buffer, fetch);
      file.write(buffer, got);
      total += got;
    }
    delay(1); // yield();
  }
  file.close();
  Serial.print("done, "); Serial.print(total); Serial.println(" bytes transferred");
}

void downloadFile_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, const char* target)
{
  // Use WiFiClientSecure class to create TLS connection
#if USE_BearSSL
  BearSSL::WiFiClientSecure client;
#else
  WiFiClientSecure client;
#endif
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
#if USE_BearSSL
  if (fingerprint) client.setFingerprint((uint8_t*)fingerprint);
#endif
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
#if defined (ESP8266) && !USE_BearSSL
  if (fingerprint)
  {
    if (client.verify(fingerprint, host))
    {
      Serial.println("certificate matches");
    }
    else
    {
      Serial.println("certificate doesn't match");
      return;
    }
  }
#endif
  Serial.print("requesting URL: ");
  Serial.println(String("https://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_Spiffs_Loader\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  bool ok = false;
  while (client.connected() || client.available())
  {
    String line = client.readStringUntil('\n');
    if (!ok)
    {
      ok = line.startsWith("HTTP/1.1 200 OK");
      if (ok) Serial.println(line);
      //if (!ok) Serial.println(line);
    }
    if (!ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!ok) return;
  uint8_t buffer[512];
#if defined(ESP8266)
  client.peekBytes(buffer, 2);
  Serial.write(buffer[0]); Serial.write(buffer[1]); Serial.println();
#endif
  size_t total = 0;
#if defined(ESP32)
  fs::File file = SPIFFS.open(String("/") + target, "w+");
#else
  fs::File file = SPIFFS.open(target, "w+");
#endif
  if (!file)
  {
    Serial.print(target); Serial.println(" open failed");
    return;
  }
  while (client.connected() || client.available())
  {
    // this doesn't work as expected, but it helps for long downloads
    int32_t start = millis();
    for (int16_t t = 0, dly = 50; t < 20; t++, dly += 50)
    {
      if (!(client.connected() || client.available())) break;
      if (client.available()) break; // read would not recover after having returned 0
      delay(dly);
    }
    if (!(client.connected() || client.available())) break;
    int32_t elapsed = millis() - start;
    if (elapsed > 250)
    {
      Serial.print("waited for available "); Serial.print(millis() - start); Serial.print(" ms @ "); Serial.println(total);
    }
    size_t available = client.available();
    if (0 == available)
    {
      Serial.print("download error: timeout on available() after "); Serial.print(total); Serial.println(" bytes");
      break; // don't hang forever
    }
    size_t fetch = available <= sizeof(buffer) ? available : sizeof(buffer);
    if (fetch > 0)
    {
      size_t got = client.read(buffer, fetch);
      file.write(buffer, got);
      total += got;
    }
    delay(1); // yield();
    if (total > 30000) delay(250); // helps for long downloads
  }
  file.close();
  Serial.print("done, "); Serial.print(total); Serial.println(" bytes transferred");
}
