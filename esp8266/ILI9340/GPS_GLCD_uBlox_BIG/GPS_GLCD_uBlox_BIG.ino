/*GPS_GLCD_uBlox Time/Temperature sketch by M. Ray Burnette 2/19/2014
  Arduino 1.5.6r2
  Binary sketch size:  19,394 (of a 30,720 byte maximum
  Data:       1,243 bytes (.data + .bss + .noinit)

  Primary Color definitions for TFT display: int_16
  ILI9340_BLACK   0x0000, ILI9340_BLUE    0x001F, ILI9340_RED     0xF800, ILI9340_GREEN   0x07E0
  ILI9340_CYAN    0x07FF, ILI9340_MAGENTA 0xF81F, ILI9340_YELLOW  0xFFE0, ILI9340_WHITE   0xFFFF
*/
// See tab/file License.h for use/rights information
#define RAWDEBUG false

#include <SPI.h>
#include "Adafruit_GPS.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
#include "GLCD.h"
#include "Utilities.h"
#include <SoftwareSerial.h>
#include <Streaming.h>

//   Connect the GPS TX (transmit) pin to Digital 3 input
SoftwareSerial mySerial(5, 19) ;  // Rx Tx (A5)
Adafruit_GPS GPS(&mySerial) ;

uint8_t last_seconds ;
uint8_t TimeZoneHour = 5 ;   // Affected as -1 for DST
uint8_t LocalHour ;
uint8_t LocalMinute ;
uint8_t LocalSecond ;
uint8_t LocalMonth ;
uint8_t LocalDay ;
uint8_t LocalYear ;

boolean PM ;
boolean DST ;                 // Daylight Savings Time: Pin A3 DST = LOW Std time = HIGH
boolean FirstTime ;           // Before satellite time is available via $GPRMC
boolean StatScreen ;          // User messages at boot up
boolean GPSECHO  ;           // set by digital pin D11
boolean Fahrenheit ;          // set by digital pin D12
float Celsius ;


void setup()  
{
  lcd.begin() ;        // 2.2" GLCD SPI 240x320 used in LANDSCAPE
  lcd.setRotation(1);  // landscape with SPI conn J2 to right side.
  lcd.fillScreen(ILI9340_BLACK);
  lcd.setCursor(0, 0);
  lcd.setTextColor(ILI9340_WHITE);
  // lcd.setTextSize(1);  // Supper Small!!! 53 char / line
  // lcd.setTextSize(2);  //  26 char / line
  lcd.setTextSize(3);     // Medium 17 char / line  10Pixels Wide x 25 Pixels / line
  //lcd.setTextSize(4);   // Large font 13 char / line
  
  analogReference(DEFAULT);
  FirstTime = true ;        // Flag to display user message one time
  StatScreen = true ;       // Flag to display user message one time

  pinMode( 6, INPUT) ;     // BAUD hi = 9600 lo = 4800
  digitalWrite( 6, HIGH);  // turn on pullup resistor

  pinMode( 7, INPUT) ;     // F/C temperature display
  digitalWrite( 7, HIGH);  // turn on pullup resistor

  pinMode( D3, INPUT) ;     // DST hi = NO low = YES
  digitalWrite( 17, HIGH);  // turn on pullup resistor

  pinMode( 4, INPUT) ;     // GPSECHO D4
  digitalWrite( 4, HIGH);  // turn on pullup resistor

  if( digitalRead(4) ) {  // D4 low for realtime GPS Debug mode echo to terminal
    GPSECHO = false ;     // Default
  } else {
    GPSECHO = true ;
    // Echo will be on physical serial port
    Serial.begin(115200) ; // connect at 115200 to minimize transfer time
  }

  if( digitalRead(6) ) {  // D6 low for 9600 BAUD GPS module
    GPS.begin(9600) ;      // Default
    if (GPSECHO) Serial << "9600 BAUD \n \r" ;
  } else {
    GPS.begin(4800) ;
    if (GPSECHO) Serial << "4800 BAUD \n \r" ;
  }

  if( digitalRead(7) ) {  // D7 low for Celsius display
      Fahrenheit = true ;    // Default
      if (GPSECHO) Serial << "Fahrenheit = ON \n \r" ;
  } else
    {
        Fahrenheit = false;
        if (GPSECHO) Serial << "Celsius = ON \n \r" ;
    }

  if( digitalRead(17) ) {  // A3 low for Daylight Savings Time
    DST = false ;          // Default
    if (GPSECHO) Serial << "DST = NO \n \r" ;
  } else {
    DST = true ;
    if (GPSECHO) Serial << "DST = YES \n \r" ;
    TimeZoneHour = TimeZoneHour - 1 ;
  }

  if(GPSECHO) Serial << "GPS Serial reception Echo Mode: \n \r" ;
  lcd.setCursor(0, 0) ;
  lcd.print("by: Ray Burnette") ;
  lcd.setCursor(0, 25) ;
  lcd.print("(c) GPS timedate") ;
  delay(2000) ;  // credits
  lcd.fillScreen(ILI9340_BLACK);
  lcd.setCursor(0, 0);
}


void loop()
{
    // read data from the GPS
    char c = GPS.read();
    if (GPSECHO)  // Raw character echo to serial monitor
      if (c) Serial.print(c);

  // if a sentence is received, check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // return to top of loop() if sentence does not verify correctly or completely
  }

  PM = false ;

  // BIG if() active only once per second after GPS decodes new GPS.seconds
  if (last_seconds != GPS.seconds)
  {
    // Do this once to notify the user
    if (FirstTime)
    {
      FirstTime = false;           // GPS signal decoding has come on line since uC reset
      lcd.fillScreen(ILI9340_BLACK);
      lcd.setTextSize(2);          //  26 char / line on GLCD
      lcd.setCursor(0, 0) ;
      lcd.print("Display will refresh and") ;
      lcd.setCursor(0, 15) ;
      lcd.print("correct when seconds= 00") ;
      delay (2000) ;
    }

    // update with new satellite data
    last_seconds =  GPS.seconds ;
    LocalHour    =  GPS.hour ;
    LocalMinute  =  GPS.minute ;
    LocalSecond  =  GPS.seconds ;
    LocalMonth   =  GPS.month ;
    LocalDay     =  GPS.day ;
    LocalYear    =  GPS.year ;

    if(RAWDEBUG)    // normally this code is not compiled
    {
      Serial.print("\n \r RAW GPS DATA: ");
      Serial << ((LocalHour<10)?"0":"") ;
      Serial.print(LocalHour, DEC);
      Serial << (":") ;
      Serial << ((LocalMinute<10)?"0":"") ;
      Serial.print (LocalMinute, DEC); 
      Serial << (":") ;
      Serial << ((LocalSecond<10)?"0":"") ;
      Serial.print(LocalSecond, DEC) ;
      Serial.print(" *** Date: ");
      Serial << ((LocalDay<10)?"0":"") ;
      Serial.print(LocalDay, DEC) ;              //DD
      Serial << "/" << ((LocalMonth<10)?"0":"") ;
      Serial.print(LocalMonth, DEC) ;            //MM
      Serial << "/20" ;
      Serial.println(LocalYear, DEC) ;           //YYYY
    }
    
    // This section always updates every second
    //  fillRect( x,  y,   w,  h, color)
    lcd.fillRect( 138, 30, 45, 30, ILI9340_BLACK);
    lcd.setTextSize(4);
    //lcd.setTextColor( ILI9340_BLUE );
    lcd.setTextColor( random(0xff, 0xffff ) );
    lcd.setCursor( 138, 30);
    if(LocalSecond < 10)
    {
      lcd.print("0") ;                   // position  12
      lcd.print(LocalSecond) ;           // position  13
    } else {
      lcd.print(LocalSecond) ;           // positions 12, 13
    }

    if(LocalHour >= TimeZoneHour)
    {
      LocalHour = LocalHour - TimeZoneHour ;
    }
    else 
    {
      // Correct for period when London is a day ahead
      LocalHour = (LocalHour + 24) - TimeZoneHour ;
      LocalDay  = LocalDay - 1 ;
      if( LocalDay == 0)  // backward a month
      {
          LocalMonth = LocalMonth - 1 ;
          LocalDay   = DaysInMonth[ LocalMonth - 1 ] ; // index 0 based
      }
      // The above may need correcting...
      // January in London still December Westward
      if( LocalMonth == 0)           // GPS months are 01 through 12
      {
        LocalMonth = 12 ;
        LocalDay   = DaysInMonth[ 11 ] ;  // lastday of December
      }
      else if( (LocalMonth == 2) && (LocalYear % 4 == 0) && (LocalDay == 28) )
      {
        // Need to deal with LocalMonth = 2 and leapyear (simple... not exhaustive)
        LocalDay = LocalDay + 1 ;
      }
    }

    // Military or Civilian time selection
    if(LocalHour >= 12)
    {
      PM = true ;
      //if(LocalHour > 12) LocalHour = LocalHour - 12 ;  // non-Mil time
    }

    // Fahrenheit or Celsium selection
    if( digitalRead(7) ) {  // D7 low for Celsius display
      Fahrenheit = true ;    // Default
    } else {
      Fahrenheit = false; }

    // Display to GLCD
    if( LocalSecond == 0 )            // full screen update every minute
    {
      lcd.fillScreen(ILI9340_BLACK);  // Screen clear GLCD
      Celsius = TempCelsius() ;
      lcd.setTextSize(4);
      lcd.setTextColor( ILI9340_CYAN );
      lcd.setCursor( 0, 0);

      if (Fahrenheit) {
        Celsius = (Celsius * 9.0) / 5.0 + 32.5 ;  // round UP
        lcd.print( Celsius, 0 ) ;          // positions 0, 1, 2? no decimal
        lcd.print(" F") ;                  // positions 2
      } else {
        lcd.print( Celsius, 1 ) ;          // positions 0, 1
        lcd.print(" C") ;                  // positions 2
      }

      // Middle pane
      lcd.setTextColor( ILI9340_GREEN );
      lcd.setTextSize(10);   
      lcd.setCursor( 10, 80) ;
      if(LocalHour < 10) lcd.print (" ") ; // position  6
      lcd.print(LocalHour) ;               // positions 6, 7
      lcd.print(":") ;                     // position  8

      if(LocalMinute < 10)
      {
        lcd.print("0") ;                     // position  9
        lcd.print(LocalMinute) ;             // position  10
       } else {
          lcd.print(LocalMinute) ;           // positions 9, 10
       }

      // Day-Of-Week, Month and Date info
      lcd.setTextColor(ILI9340_YELLOW);
      lcd.setTextSize(4);
      // Day Of Week is shown in Top pane, right
      lcd.setCursor(235, 0);                                       // upper section, right
      lcd.print(DayOfWeek[dow(LocalYear, LocalMonth, LocalDay)]) ; // DayOfWeek
      // Month Name and Date and Year are shown in lower pane
      lcd.setCursor(35, 195);
      lcd.print(NameOfMonth[LocalMonth - 1]) ;                     // positions 6 - 8
      lcd.print(" ") ;                                             // position  9
      if(LocalDay < 10) {
        lcd.print(" ") ;                                           // position  10
        lcd.print(LocalDay) ;
      } else {
        lcd.print(LocalDay) ;                                      // positions 10, 11
      }
      lcd.print(" 20") ;                                           // fix to hardcode requires .cpp changes in LocalYear typing
      lcd.print(LocalYear) ;
    }

    if(RAWDEBUG)    // normally this code is not compiled
    {
      Serial.print("DISPLAYED DATA: ");
      Serial << ((LocalHour<10)?"0":"") ;
      Serial.print(LocalHour, DEC);
      Serial << (":") ;
      Serial << ((LocalMinute<10)?"0":"") ;
      Serial.print (LocalMinute, DEC); 
      Serial << (":") ;
      Serial << ((LocalSecond<10)?"0":"") ;
      Serial.print(LocalSecond, DEC) ;
      Serial.print(" *** Date: ");
      Serial << ((LocalDay<10)?"0":"") ;
      Serial.print(LocalDay, DEC) ;              //DD
      Serial << "/" << ((LocalMonth<10)?"0":"") ;
      Serial.print(LocalMonth, DEC) ;            //MM
      Serial << "/20" ;
      Serial.println(LocalYear, DEC) ;           //YYYY
    }
    
  }  // close BIG IF
    else if( FirstTime && StatScreen )    // This will only happen once
    {
        lcd.fillScreen(ILI9340_BLACK);
        lcd.setCursor(0, 0);
        lcd.print( "Wait for GPS ... ") ;
        lcd.print( "Up to 15 minutes!") ;
        if ( Fahrenheit ) {
            lcd.println( "Fahrenheit" ) ;
          } else {
            lcd.println( "Celsius") ; }

        if ( DST ) {
          lcd.println( "DST = On" );
        } else {
          lcd.println( "DST = OFF"); }
          
      delay( 2000 ) ;
      StatScreen = false ;
      lcd.fillScreen(ILI9340_BLACK);
      lcd.setCursor(0, 0);
    }
} // loop




