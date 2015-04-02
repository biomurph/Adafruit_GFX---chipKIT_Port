/*********************************************************************
This is an Arduino library for our Monochrome SHARP Memory Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1393

These displays use SPI to communicate, 3 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution


Ported to chipKIT by Joel Murphy. Spring, 2015
    Sharp Memory Display From EmbeddedArtists
    http://www.mouser.com/ProductDetail/Embedded-Artists/EA-LCD-007/?qs=%2fha2pyFaduiVZdW7yspVAF2x15uTaSR%252by3ES49YFBdtKf4Fo6ojIjw%3d%3d

    >>>>>  NOTE: This code does not implement software toggle of VCOM!!!
    >>>>>  LCD Module HACK: Solder Rework! 
    >>>>>  Remove 0ohm resistor R9, and jump the R2 pads to pull EXTMODE pin HIGH
    >>>>>  Jump the R7 pads to connect EXTCOMIN pin to connecter
    >>>>>  Provide a clock (<1Hz) to EXTCOMIN pin
    >>>>>  For the clock, I am using a 7414 (single Inverter)74AHC1G14GV:
    >>>>>  http://www.mouser.com/ProductDetail/NXP-Semiconductors/74AHC1G14GV125/?qs=P62ublwmbi%2FTMzPIGwYKEw%3D%3D
    >>>>>  f = 1/0.55*R*C

    Uses DSPI for SPI bus control
*********************************************************************/


#include <DSPI.h>
#include "WProgram.h"
#include <stdint.h>
#include <Adafruit_GFX.h>

// LCD Dimensions
#define SHARPMEM_LCDWIDTH       (96)
#define SHARPMEM_LCDHEIGHT      (96) 



class Adafruit_SharpMem : public Adafruit_GFX {
 public:
  DSPI0 spi;  // use DSPI library
  Adafruit_SharpMem(int ss, int disp);
  void begin(void);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  uint8_t getPixel(uint16_t x, uint16_t y);
  void clearDisplay();
  void refresh(void);
  void disableLCD();
  void initLCD();
  uint8_t reverse(uint8_t v);

 private:
  int _ss, _disp;

};
