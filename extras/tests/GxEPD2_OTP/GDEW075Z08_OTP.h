#ifndef _GDEW075Z08_OTP_H_
#define _GDEW075Z08_OTP_H_

#include <GxEPD2_GFX.h>

class GDEW075Z08_OTP
{
  public:
    GDEW075Z08_OTP(GxEPD2_GFX& gfx) : display(gfx) {};
    void check();
    void read_TSC();
    void read_temperature();
    void read_revision();
    void read_status();
    void read_vcom();
    void read_otp();
    void read_LUT_TR(int n);
    void read_LUT(uint16_t &addr);
    void read_LUT_KW(uint16_t &addr);
  private:
    GxEPD2_GFX& display;
};

#endif
