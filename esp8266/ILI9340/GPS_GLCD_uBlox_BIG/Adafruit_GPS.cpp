/*
See rights and use declaration in License.h
This library has been modified from original by Ray Burnette 12/2013
*/
#include "Adafruit_GPS.h"

// how long are max NMEA lines to parse?
#define MAXLINELENGTH 120

// we double buffer: read one line in and leave one for the main program
volatile char line1[MAXLINELENGTH];
volatile char line2[MAXLINELENGTH];
// our index into filling the current line
volatile uint8_t lineidx=0;
// pointers to the double buffers
volatile char *currentline;
volatile char *lastline;
volatile boolean recvdflag;
volatile boolean inStandbyMode;
volatile boolean ColdGPS = true ;


boolean Adafruit_GPS::parse(char *nmea) {
  // do checksum check
  // first look if we even have one
  if (nmea[strlen(nmea)-4] == '*') {
    uint16_t sum = parseHex(nmea[strlen(nmea)-3]) * 16;
    sum += parseHex(nmea[strlen(nmea)-2]);
    
    // check checksum 
    for (uint8_t i=1; i < (strlen(nmea)-4); i++) {
      sum ^= nmea[i];
    }
    
    if (sum != 0) {
      // bad checksum :(
      //return false;
    }
  }


  if (strstr(nmea, "$GPRMC")) {
   // found RMC
    char *p = nmea;

    // get time
    p = strchr(p, ',')+1;
    float timef = atof(p);
    uint32_t time = timef;
    hour = time / 10000;
    minute = (time % 10000) / 100;
    seconds = (time % 100);

    milliseconds = fmod(timef, 1.0) * 1000;
    p = strchr(p, ',')+1;  // A/V?
    p = strchr(p, ',')+1;  // lat
    p = strchr(p, ',')+1;  // N/S?
    p = strchr(p, ',')+1;  // lon
    p = strchr(p, ',')+1;  // E/W?
    p = strchr(p, ',')+1;  // speed
    p = strchr(p, ',')+1;  // angle

    p = strchr(p, ',')+1;
    uint32_t fulldate = atof(p);
    day = fulldate / 10000;
    month = (fulldate % 10000) / 100;
    year = (fulldate % 100);
    // some older GPS modules only send GGA messages until full-GPS sync
    // so, only want to use the GGA sentences until RMC is available as
    // GGA only has time info and not date
    ColdGPS = false ;
    return true;
  }
  else if (ColdGPS && strstr(nmea, "$GPGGA")) {
    // found GGA
    char *p = nmea;
    // get time
    p = strchr(p, ',')+1;
    float timef = atof(p);
    uint32_t time = timef;
    hour = time / 10000;
    minute = (time % 10000) / 100;
    seconds = (time % 100);
    milliseconds = fmod(timef, 1.0) * 1000;
    return true;
  }
    return false;
}

char Adafruit_GPS::read(void) {
  char c = 0;
  
  if (paused) return c;

    if(!gpsSwSerial->available()) return c;
    c = gpsSwSerial->read();


  if (c == '\n' || c == '\r' || c == '$') {
    currentline[lineidx] = 0;
    lineidx = 0;
    recvdflag = true;

    if (currentline == line1) {
      currentline = line2;
      lastline = line1;
    } else {
      currentline = line1;
      lastline = line2;
    }
  }

  currentline[lineidx++] = c;
  if (lineidx >= MAXLINELENGTH)
    lineidx = MAXLINELENGTH-1;

  return c;
}

Adafruit_GPS::Adafruit_GPS(SoftwareSerial *ser)
{
  common_init();     // Set everything to common state, then...
  gpsSwSerial = ser; // ...override gpsSwSerial with value passed.
}

// Initialization code used by all constructor types
void Adafruit_GPS::common_init(void) {
  gpsSwSerial = NULL; // Set both to NULL, then override correct
  recvdflag   = false;
  paused      = false;
  lineidx     = 0;
  currentline = line1;
  lastline    = line2;
  hour = minute = seconds = year = month = day = fixquality = satellites = 0; // uint8_t
  milliseconds = 0; // uint16_t
}

void Adafruit_GPS::begin(uint16_t baud) {
  if(gpsSwSerial) gpsSwSerial->begin(baud);
  delay(10);
}

boolean Adafruit_GPS::newNMEAreceived(void) {
  return recvdflag;
}

void Adafruit_GPS::pause(boolean p) {
  paused = p;
}

char *Adafruit_GPS::lastNMEA(void) {
  recvdflag = false;
  return (char *)lastline;
}

// read a Hex value and return the decimal equivalent
uint8_t Adafruit_GPS::parseHex(char c) {
    if (c < '0')
      return 0;
    if (c <= '9')
      return c - '0';
    if (c < 'A')
       return 0;
    if (c <= 'F')
       return (c - 'A')+10;
}

boolean Adafruit_GPS::waitForSentence(char *wait4me, uint8_t max) {
  char str[20];
  uint8_t i=0;
  while (i < max) {
    if (newNMEAreceived()) { 
      char *nmea = lastNMEA();
      strncpy(str, nmea, 20);
      str[19] = 0;
      i++;

      if (strstr(str, wait4me))
	return true;
    }
  }

  return false;
}


