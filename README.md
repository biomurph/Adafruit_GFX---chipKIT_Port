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
    
###drawPixel(x positoin, y position, color)

###drawLine(start x, start y, end x, end y, color)
