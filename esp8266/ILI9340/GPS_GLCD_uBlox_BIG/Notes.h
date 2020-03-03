/*
// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
                  // J2 header pin # on GLCD
#define _sclk 13  // 7
#define _miso 12  // 9
#define _mosi 11  // 6
#define _cs   10  // 3
#define _dc    9  // 5
#define _rst   8  // 4
                                                                Vcc *--/\/\/\/\---*---/\/\/\/\/---xGnd
                                                                        10K 1%    |     10K NTC
                                                                                  |
                                                                                  |__ TO A0 on uC


                                      +-\/-+
Reset                           PC6  1|    |28  PC5 (AI 5)
Rx                        (D 0) PD0  2|    |27  PC4 (AI 4)
Tx                        (D 1) PD1  3|    |26  PC3 (AI 3)      Digital: Daylight Savings Time (Low = True)
                          (D 2) PD2  4|    |25  PC2 (AI 2)
                     PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
GPSECHO Low=true          (D 4) PD4  6|    |23  PC0 (AI 0)      Junction of 10K NTC Thermistor & Resistor 10K
+5                              VCC  7|    |22  GND             GND
GND                             GND  8|    |21  AREF            +5 and bypass cap
XTL1                            PB6  9|    |20  AVCC            +5
XTL2                            PB7 10|    |19  PB5 (D 13)      J2#7/SCK
GPS serial 9600 in   PWM+ (D 5) PD5 11|    |18  PB4 (D 12)      J2#9/MISO
BAUD (low=4800)      PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM  J2#6/MOSI (SDI)
F/C                       (D 7) PD7 13|    |16  PB2 (D 10) PWM  J2#3/CS
J2#4/Reset                (D 8) PB0 14|    |15  PB1 (D  9) PWM  J2#5/DC
                                      +----+
 
             Color definitions for TFT SPI 2.2" Display
                    ILI9340_BLACK   0x0000
                    ILI9340_BLUE    0x001F
                    ILI9340_RED     0xF800
                    ILI9340_GREEN   0x07E0
                    ILI9340_CYAN    0x07FF
                    ILI9340_MAGENTA 0xF81F
                    ILI9340_YELLOW  0xFFE0  
                    ILI9340_WHITE   0xFFFF

    The following state-pins on the uC will control feature modes:
   -------------------------------------------------------------------
Physical
  328  Log  Arduino
  Pin  Port RefName    Signal usage for GPS time-date
  ___  ___  _______    _______________________________________________
   15  PB1 (D  9)      GPS serial input @BAUD
   16  PB2 (D 10)      BAUD (hi = 9600 lo = 4800)
   17  PB3 (D 11)      DEBUG (hi = OFF lo = on)
   18  PB4 (D 12)      F/C MODE (hi = F lo = C)
   19  PB5 (D 13)      LED Cycle On-Off at 1 second
   23  PC0 (A  0)      Junction of 10K NTC Thermistor & Resistor 10K
   26  PC3 (A  3)      Digital - Daylight Savings Time (Lo = True)      D17
   27  PC4 (A  4)      Digital - Debug Mode                             D18
   28  PC5 (A  5)      Digital - Tx (Reserved)                          D19
   
Analog used as DIGITAL mapping:
  Pin 14 = Analog in 0
  Pin 15 = Analog in 1
  Pin 16 = Analog in 2
  Pin 17 = Analog in 3
  Pin 18 = Analog in 4
  Pin 19 = Analog in 5


0,0
----------------------------------------------------------------------------> 319
| nnnT                                                                 DOW
|              lcd.fillRect( 0,  0, 319, 59, 0);     // blank top
|
|
|<- 059 
|
|              lcd.fillRect( 0, 60, 319, 114, 0);     // blank middle
|                              HH:MM:SS A
|
|
|
|<- 174
|              lcd.fillRect( 0, 175, 319, 64, 0);     // blank lower
|
| MON DD YYYY
|
|<- 239
 */


