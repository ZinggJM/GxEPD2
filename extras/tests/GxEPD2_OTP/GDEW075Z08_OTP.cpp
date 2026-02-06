#include "GDEW075Z08_OTP.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

void GDEW075Z08_OTP::check()
{
  //read_TSC();
  read_temperature();
  //  read_revision();
  //  read_status();
  //read_vcom();
  read_otp();
  //  for (int i = 0; i < 10; i++) // 10 temperature ranges
  //  {
  //    read_LUT_TR(i);
  //  }
}

void GDEW075Z08_OTP::read_TSC()
{
  display.epd2._writeCommand(0x40);
  display.epd2._waitWhileBusy("read_TSC time");
  uint8_t msb = display.epd2._readData();
  uint8_t lsb = display.epd2._readData();
  Serial.print("read_TSC: 0x"); Serial.print(msb, HEX); Serial.print(", 0x"); Serial.println(lsb, HEX);
}

void GDEW075Z08_OTP::read_temperature()
{
  display.epd2._writeCommand(0x43);
  display.epd2._waitWhileBusy("read_temperature time");
  uint8_t msb = display.epd2._readData();
  uint8_t lsb = display.epd2._readData();
  Serial.print("read_temperature: "); Serial.print(msb); Serial.print(", "); Serial.println(lsb);
}

void GDEW075Z08_OTP::read_revision()
{
  display.epd2._writeCommand(0x70);
  uint8_t rv = display.epd2._readData();
  Serial.print("read_revision: "); Serial.println(rv);
}

void GDEW075Z08_OTP::read_status()
{
  display.epd2._writeCommand(0x71);
  uint8_t rv = display.epd2._readData();
  Serial.print("read_status: 0x"); Serial.println(rv, HEX);
}

void GDEW075Z08_OTP::read_vcom()
{
  display.epd2._writeCommand(0x81);
  uint8_t rv = display.epd2._readData();
  Serial.print("read_vcom: 0x"); Serial.println(rv, HEX);
}

void GDEW075Z08_OTP::read_otp()
{
  uint16_t addr = 0;
  uint8_t value = 0;
  Serial.println("read_otp");
  display.epd2._writeCommand(0xA2);
  display.epd2._waitWhileBusy("read_otp");
  value = display.epd2._readData(); addr++;
  Serial.print("Check Code: 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("LUT Version: 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 0 (TB0): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 1 (TB1): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 2 (TB2): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 3 (TB3): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 4 (TB4): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 5 (TB5): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 6 (TB6): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 7 (TB7): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 8 (TB8): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Temperature Boundary 9 (TB9): 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  //Serial.print("Temperature Boundary 10 (TB10): 0x"); Serial.println(value, HEX);
  //value = display.epd2._readData(); addr++;
  Serial.print("CDS : Check Code 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : PSR 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : PFS 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : BTST 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : BTST 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : BTST 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : KWOPT 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : CDI 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : CDI 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : TCON 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : TRES 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : TRES 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : TRES 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : GSST 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : GSST 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : GSST 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : PWS 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("CDS : LVSEL 0x"); Serial.println(value, HEX);
  Serial.print("@ 0x"); Serial.println(addr, HEX);
  for (; addr < 0x49; addr++)
  {
    if (0 == addr % 6) Serial.print(" ");
    if (0 == addr % 12) Serial.println();
    uint8_t rv = display.epd2._readData();
    Serial.print("0x"); Serial.print(rv, HEX); Serial.print(", ");
  }
  Serial.println();
  for (int i = 0; i < 9; i++) // 10 temperature ranges, 9 used for 3-color
  {
    read_LUT(addr);
  }
  //read_LUT_KW(addr); // not used on this panel
}

void GDEW075Z08_OTP::read_LUT_TR(int n)
{
  uint8_t value = 0;
  int addr = 0x0049 + n * (0x0140 - 0x0049); // 0x0049: @LUT_TR0, 0x140: @LUT_TR1
  Serial.print("read_LUT_TR("); Serial.print(n); Serial.print(")"); Serial.print(" @ 0x"); Serial.println(addr, HEX);
  display.epd2._writeCommand(0xA2);
  display.epd2._waitWhileBusy();
  for (int skip = 0; skip < addr; skip++)
  {
    uint8_t v = display.epd2._readData();
  }
  value = display.epd2._readData(); addr++;
  Serial.print("Frame Rate[3:0],VCOM_HV,VG Voltage[2:0] 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("0b, VCEND, VDH Voltage[5:0]             0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("BDEND[1:0], VDL Voltage [5:0]           0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("XON[9:8], VDHR Voltage [5:0]            0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("XON [7:0]                               0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("00b, VCOM_DC[5:0]                       0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("??                                      0x"); Serial.println(value, HEX);
  Serial.print("@ 0x"); Serial.println(addr, HEX);
  for (int lut = 0; lut < 4; lut++) // there are 4 groups for UC8179 in KWR (LUTC, LUTR,  LUTW, LUTK)
  {
    for (int r = 0; r < 10; r++)     // there are 10 entries per group
    {
      for (int c = 0; c < 6; c++)   // each entry has 6 values
      {
        uint8_t rv = display.epd2._readData(); addr++;
        Serial.print("0x"); Serial.print(rv, HEX); Serial.print(", ");
      }
      Serial.println();
    }
    Serial.println();
  }
  Serial.print("@ 0x"); Serial.println(addr, HEX);
  Serial.println();
}

void GDEW075Z08_OTP::read_LUT(uint16_t &addr)
{
  uint8_t value = 0;
  uint16_t n = addr / (0x0140 - 0x0049); // 0x0049: @LUT_TR0, 0x140: @LUT_TR1
  Serial.print("read_LUT "); Serial.print(n); Serial.print(" @ 0x"); Serial.println(addr, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Frame Rate[3:0],VCOM_HV,VG Voltage[2:0] 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("0b, VCEND, VDH Voltage[5:0]             0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("BDEND[1:0], VDL Voltage [5:0]           0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("XON[9:8], VDHR Voltage [5:0]            0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("XON [7:0]                               0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("00b, VCOM_DC[5:0]                       0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("??                                      0x"); Serial.println(value, HEX);
  Serial.print("@ 0x"); Serial.println(addr, HEX);
  for (int lut = 0; lut < 4; lut++) // there are 4 groups for UC8179 in KWR (LUTC, LUTR,  LUTW, LUTK)
  {
    for (int r = 0; r < 10; r++)     // there are 10 entries per group
    {
      for (int c = 0; c < 6; c++)   // each entry has 6 values
      {
        uint8_t rv = display.epd2._readData(); addr++;
        Serial.print("0x"); Serial.print(rv, HEX); Serial.print(", ");
      }
      Serial.println();
    }
    Serial.println();
  }
  Serial.print("@ 0x"); Serial.println(addr, HEX);
  Serial.println();
}

void GDEW075Z08_OTP::read_LUT_KW(uint16_t &addr)
{
  uint8_t value = 0;
  uint16_t n = addr / (0x0140 - 0x0049); // 0x0049: @LUT_TR0, 0x140: @LUT_TR1
  Serial.print("read_LUT_KW "); Serial.print(n); Serial.print(" @ 0x"); Serial.println(addr, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("Frame Rate[3:0],VCOM_HV,VG Voltage[2:0] 0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("0b, VCEND, VDH Voltage[5:0]             0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("BDEND[1:0], VDL Voltage [5:0]           0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("XON[9:8], VDHR Voltage [5:0]            0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("XON [7:0]                               0x"); Serial.println(value, HEX);
  value = display.epd2._readData(); addr++;
  Serial.print("00b, VCOM_DC[5:0]                       0x"); Serial.println(value, HEX);
  //value = display.epd2._readData(); addr++;
  //Serial.print("??                                      0x"); Serial.println(value, HEX);
  Serial.print("@ 0x"); Serial.println(addr, HEX);
  for (int lut = 0; lut < 5; lut++) // there are 5 groups for UC8179 in KW (LUTC, LUTWW,  LUTKW, LUTWK, LUTKK)
  {
    for (int r = 0; r < 7; r++)     // there are 7 entries per group
    {
      for (int c = 0; c < 6; c++)   // each entry has 6 values
      {
        uint8_t rv = display.epd2._readData(); addr++;
        Serial.print("0x"); Serial.print(rv, HEX); Serial.print(", ");
      }
      Serial.println();
    }
    Serial.println();
  }
  Serial.print("@ 0x"); Serial.println(addr, HEX);
  Serial.println();
}
