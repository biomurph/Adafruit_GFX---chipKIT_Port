# Adafruit_GFX---chipKIT_Port
Another portage of Adafruit_GFX library. This one is specific to the Sharp Mem 1.3" BW Screen

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
    >>>>>  f = 1/0.55*R*C [my R = 10K, C = 47uF]

Uses DSPI for SPI bus control.
Constructor takes two parameters: LCD_SS, DISP
  LCD_SS is the Slave Select pin for the LCD. Can be any pin other than 11,12,13
    watch out for pin 10 on Joel's board! it is acting funny
  DISP pin is the Display control pin for the LCD. Can be any pin other than 11,12,13
  
Example included is a copy of the Adafruit example, modified to run with this library.

## Drawing Functions:
    color parameter can be BLACK=0 or WHITE=1
    screen is 96x96 pixels
    draw functions write bits to the sharpmem_buffer
    print to the LCD by calling refresh(); after setting up the buffer the way you like
    
###refresh(void)
this does the actual writing of pixel data to the screen. call this after callin draw or text functions to display

###drawPixel(x positoin, y position, color)

###fillScreen(color)

###drawLine(start x, start y, end x, end y, color)

###drawRect(corner x, corner y, width, height, color)
start corner is upper left

###fillRect(corner x, corner y, width, height, color)
start corner is still upper left

###drawRoundRect(corner x, corner y, width, height, radius, color)

###fillRoundRect(corner x, corner y, width, height, radius, color)

###drawTriangle(corner1 x, corner1 y, corner2 x, corner2 y, corner3 x, corner3 y, color)

###fillTriangle(corner1 x, corner1 y, corner2 x, corner2 y, corner3 x, corner3 y, color)

###drawCircle(center x, center y, radius, color)

###fillCircle(center x, center y, radius, color)

###drawBitmap(corner x, corner y, *bitmapArray, width, height, color)
it's better to keep your bitmap array in progmem, if you can
-  Image2Code
This is a handy Java GUI utility to convert a BMP file into the array code necessary to display the image with the drawBitmap function.  Check out the code at ehubin's GitHub repository:
     https://github.com/ehubin/Adafruit-GFX-Library/tree/master/Img2Code

###drawXBitmap(corner x, corner y, *bitmapArray, width, height, color)
Draw XBitMap Files (*.xbm), exported from GIMP,
Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
C Array can be directly used with this function
See the pull request here for more details:
     https://github.com/adafruit/Adafruit-GFX-Library/pull/31
     
##Text

###setTextSize(heightMultiplier)
default font is 8 pixels tall. This function multiplies 8*heightMultiplier

###setCursor(x position, y position)

###setTextColor(color)

###setTextColor(text color, background color)
sets the text and background. background extends only as big as the charaters printed

###setTextWrap(boolean)
default is true, so text will wrap at the right edge of the screen

###write(char)
draws the ascii character specified

###drawChar(x position, y position, char, text color, backgroudn color, text size)
This function is called by the write(char) command

