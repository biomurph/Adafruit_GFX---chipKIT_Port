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

Ported to chipKIT by Joel Murphy for his own purposes. Spring, 2015

*********************************************************************/

#include "Adafruit_SharpMem.h"

/**************************************************************************
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

 **************************************************************************/

#define SHARPMEM_BIT_WRITECMD   (0x80)
#define SHARPMEM_BIT_VCOM       (0x40)
#define SHARPMEM_BIT_CLEAR      (0x20)
#define TOGGLE_VCOM             do { _sharpmem_vcom = _sharpmem_vcom ? 0x00 : SHARPMEM_BIT_VCOM; } while(0);
#define DYNAMIC 0x80  // reverse of 0x01  Write enable (allows for changing lines)
#define STATIC  0x00  // reverse of 0x00  low power mode. Refresh after 2hrs
#define CLEAR 0x20  // reverse of 0x04  CLEAR_ALL (all pixels white)



byte sharpmem_buffer[(SHARPMEM_LCDWIDTH * SHARPMEM_LCDHEIGHT) / 8];

/* ************* */
/* CONSTRUCTORS  */
/* ************* */
Adafruit_SharpMem::Adafruit_SharpMem(int ss, int disp) :
Adafruit_GFX(SHARPMEM_LCDWIDTH, SHARPMEM_LCDHEIGHT) {

   _disp = disp;
   _ss = ss;
  pinMode(10,OUTPUT); digitalWrite(10,HIGH);
  pinMode(_disp,OUTPUT); disableLCD();  // turn off display
  spi.begin();  //_ss);
  spi.setSpeed(1000000);    // max clock 1MHz
  spi.setMode(DSPI_MODE0);  // 
  spi.setPinSelect(_ss);
  spi.setSelect(LOW);       // SS idles LOW -wierd, I know


}

void Adafruit_SharpMem::begin() {
  setRotation(2);
  initLCD();
}


// /* ************** */
// /* PUBLIC METHODS */
// /* ************** */

// // 1<<n is a costly operation on AVR -- table usu. smaller & faster
static const uint8_t PROGMEM
  set[] = {  1,  2,  4,  8,  16,  32,  64,  128 },
  clr[] = { ~1, ~2, ~4, ~8, ~16, ~32, ~64, ~128 };

/**************************************************************************/
/*! 
    @brief Draws a single pixel in image buffer

    @param[in]  x
                The x position (0 based)
    @param[in]  y
                The y position (0 based)
*/
/**************************************************************************/
void Adafruit_SharpMem::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

  switch(rotation) {
   case 1:
    swap(x, y);
    x = WIDTH  - 1 - x;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    swap(x, y);
    y = HEIGHT - 1 - y;
    break;
  }

  if(color) {
    sharpmem_buffer[(y*SHARPMEM_LCDWIDTH + x) / 8] |=
      pgm_read_byte(&set[x & 7]);
  } else {
    sharpmem_buffer[(y*SHARPMEM_LCDWIDTH + x) / 8] &=
      pgm_read_byte(&clr[x & 7]);
  }
}

/**************************************************************************/
/*! 
    @brief Gets the value (1 or 0) of the specified pixel from the buffer

    @param[in]  x
                The x position (0 based)
    @param[in]  y
                The y position (0 based)

    @return     1 if the pixel is enabled, 0 if disabled
*/
/**************************************************************************/
uint8_t Adafruit_SharpMem::getPixel(uint16_t x, uint16_t y)
{
  if((x >= _width) || (y >= _height)) return 0; // <0 test not needed, unsigned

  switch(rotation) {
   case 1:
    swap(x, y);
    x = WIDTH  - 1 - x;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    swap(x, y);
    y = HEIGHT - 1 - y;
    break;
  }

  return sharpmem_buffer[(y*SHARPMEM_LCDWIDTH + x) / 8] &
    pgm_read_byte(&set[x & 7]) ? 1 : 0;
}

/**************************************************************************/
/*! 
    @brief Clears the screen
*/
/**************************************************************************/
void Adafruit_SharpMem::clearDisplay() 
{
  memset(sharpmem_buffer, 0xff, (SHARPMEM_LCDWIDTH * SHARPMEM_LCDHEIGHT) / 8);

  spi.setSelect(HIGH);
        
  spi.transfer(CLEAR);  // send clear command
  spi.transfer(0x00);   // send trailer
        
  spi.setSelect(LOW);

}

void Adafruit_SharpMem::disableLCD(){
  digitalWrite(_disp,LOW);
}

void Adafruit_SharpMem::initLCD(){
  delay(50);  // waiting for something (power up?)
  clearDisplay();
  digitalWrite(_disp,HIGH);
}

uint8_t Adafruit_SharpMem::reverse(uint8_t v) {
  v = ((v >> 1) & 0x55) | ((v & 0x55) << 1); /* swap odd/even bits */
  v = ((v >> 2) & 0x33) | ((v & 0x33) << 2); /* swap bit pairs */
  v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4); /* swap nibbles */
  return v;
}

/**************************************************************************/
/*! 
    @brief Renders the contents of the pixel buffer on the LCD
*/
/**************************************************************************/
void Adafruit_SharpMem::refresh(void) 
{
  uint16_t i, totalbytes, currentline, oldline;  
  totalbytes = (SHARPMEM_LCDWIDTH * SHARPMEM_LCDHEIGHT) / 8;


spi.setSelect(HIGH);
  int byteCounter = 0;
  spi.transfer(DYNAMIC);      // send the write command
  for(int i=1; i<=96; i++){   // 96 lines in Y direction
    byte line = reverse(i);   // little-endian, please
    spi.transfer(line);       // send line number
    for(int i=1; i<=12; i++){
      spi.transfer(reverse(sharpmem_buffer[byteCounter]));   byteCounter++;  // send line data
    }
    spi.transfer(0x00);     // send trailer
  }
  spi.transfer(0x00);     // send final trailer

  spi.setSelect(LOW);
}

 