/*
  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  See rights declaration in License.h
*/
 
 #if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// These SPI pins are for the UNO 328 ---> Change for 32U4, 1284, 2560 etc.
//       UNO328   // J2 header pin # on GLCD
#define _sclk 13  // 7
#define _miso 12  // 9
#define _mosi 11  // 6
#define _cs   10  // 3
#define _dc    9  // 5
#define _rst   8  // 4
  
// Using software SPI is really not suggested, its incredibly slow
// Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
// Use hardware SPI
Adafruit_ILI9340 lcd = Adafruit_ILI9340(_cs, _dc, _rst);


