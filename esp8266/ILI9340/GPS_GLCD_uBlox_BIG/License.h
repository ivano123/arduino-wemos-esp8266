
// License.h
// All tab licenses are here

/***********************************
This is the Adafruit GPS library - the ultimate GPS library
for the ultimate GPS module!

Tested and works great with the Adafruit Ultimate GPS module
using MTK33x9 chipset
    ------> http://www.adafruit.com/products/746
Pick one up today at the Adafruit electronics shop 
and help support open source hardware & software! -ada

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

****************************************/

/***************************************************
  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 
 /*
Roughly based on Adafruit example sketch: "parsing"
Utilizes the (modified) Adafruit_GPS library https://github.com/adafruit/Adafruit-GPS-Library
20131204: GPS TimeDate with Temperature
  Heavily mucked by M. Ray Burnette to simply use (most) any dumb serial TTL GPS for time-date
  Tested with unit#28146 Parallax module @4800 BAUD http://www.abra-electronics.com/products/28146-Parallax-GPS-Receiver-Module.html
  Tested with uBlox and external antenna @9600 BAUD http://www.ebay.com/itm/390647042336
  Tested with uBlox and internal antenna @9600 BAUD http://www.ebay.com/itm/181219728986

*/
