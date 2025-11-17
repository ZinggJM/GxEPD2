// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires Zephyr RTOS 4.2.1 with SPI and GPIO drivers
//
// Ported to Zephyr RTOS from Arduino library
// Original Author: Jean-Marc Zingg
// Zephyr Port: 2025
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_H_
#define _GxEPD2_H_

#include <stdint.h>

namespace GxEPD2
{
  // Panel types enumeration
  enum Panel
  {
    GDEP015OC1,   // 1.54" b/w
    GDEW0154T8,   // 1.54" b/w, replacement for GDEP015OC1
    GDEH0154D67,  // 1.54" b/w
    GDE0213B1,    // 2.13" b/w
    GDEH0213B72,  // 2.13" b/w, replacement for GDE0213B1
    GDEH0213B73,  // 2.13" b/w, for 2.13" flexible display
    GDEW0213I5F,  // 2.13" b/w flexible
    GDEW0213T5D,  // 2.13" b/w
    GDEW0213M21,  // 2.13" b/w
    GDEW026T0,    // 2.6" b/w
    GDEW026M01,   // 2.6" b/w
    GDEH029A1,    // 2.9" b/w
    GDEW029T5,    // 2.9" b/w
    GDEW029T5D,   // 2.9" b/w
    GDEW029M06,   // 2.9" b/w
    GDEW027W3,    // 2.7" b/w
    GDEW027C44,   // 2.7" b/w/r
    GDEW0371W7,   // 3.7" b/w
    GDEW042T2,    // 4.2" b/w
    GDEW042Z15,   // 4.2" b/w/r
    ACeP565,      // 5.65" 7-color
    GDEW0583T7,   // 5.83" b/w
    GDEW0583T8,   // 5.83" b/w, replacement for GDEW0583T7
    GDEW075T8,    // 7.5" b/w
    GDEW075T7,    // 7.5" b/w, older version
    GDEW075Z09,   // 7.5" b/w/r
    GDEW075Z08,   // 7.5" b/w/r, older version
    ED060SC4,     // 6" b/w for IT8951
    ED060KC1,     // 6" b/w for IT8951
    ED078KC2,     // 7.8" b/w for IT8951
    ES103TC1      // 10.3" b/w for IT8951
  };
}

#endif
