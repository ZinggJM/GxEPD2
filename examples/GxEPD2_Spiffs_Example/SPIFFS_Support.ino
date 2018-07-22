#if defined(ESP32)

#include "FS.h"
#include "SPIFFS.h"

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void listFiles()
{
  listDir(SPIFFS, "/", 0);
}

#elif defined(ESP8266)

/*====================================================================================
  This sketch contains support functions for the ESP6266 SPIFFS filing system

  Created by Bodmer 15th Jan 2017
  ==================================================================================*/

//====================================================================================
//                 Print a SPIFFS directory list (root directory)
//====================================================================================

void listFiles(void) {
  Serial.println();
  Serial.println("SPIFFS files found:");

  fs::Dir dir = SPIFFS.openDir(""); // Root directory
  String  line = "=====================================";
  uint32_t totalBytes = 0;

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  while (dir.next()) {
    String fileName = dir.fileName();
    Serial.print(fileName);
    int spaces = 25 - fileName.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");
    fs::File f = dir.openFile("r");
    Serial.print(f.size()); Serial.println(" bytes");
    totalBytes += f.size();
  }
  Serial.println(); Serial.print("Total = ");
  int spaces = 25 - 8; // Tabulate nicely
  while (spaces--) Serial.print(" ");
  Serial.print(totalBytes); Serial.println(" bytes");

  Serial.println(line);
  Serial.println();
  delay(1000);
}
//====================================================================================

#else

void listFiles()
{
  Serial.println("SPIFFS listFiles() not implemented");
}
#endif
