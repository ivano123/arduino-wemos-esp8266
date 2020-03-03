// See rights and use declaration in License.h
#include <Arduino.h>
#ifndef _ADAFRUIT_GPS_H
  #define _ADAFRUIT_GPS_H

  #include <SoftwareSerial.h>

  // how long to wait when we're looking for a response
  #define MAXWAITSENTENCE 5

  class Adafruit_GPS {
   public:
    void begin(uint16_t baud); 

    Adafruit_GPS(SoftwareSerial *ser); // Constructor when using SoftwareSerial
    // Adafruit_GPS(HardwareSerial *ser); // Constructor when using HardwareSerial
    char *lastNMEA(void);
    boolean newNMEAreceived();
    void common_init(void);
    void sendCommand(char *);
    void pause(boolean b);
    boolean parseNMEA(char *response);
    uint8_t parseHex(char c);
    char read(void);
    boolean parse(char *);
    void interruptReads(boolean r);
    boolean wakeup(void);
    boolean standby(void);
    uint8_t hour, minute, seconds, year, month, day;
    uint16_t milliseconds;
    char lat, lon, mag;
    boolean fix;
    uint8_t fixquality, satellites;
    boolean waitForSentence(char *wait, uint8_t max = MAXWAITSENTENCE);

    private:
      boolean paused;
      uint8_t parseResponse(char *response);

    SoftwareSerial *gpsSwSerial;
    //HardwareSerial *gpsHwSerial;
  };
#endif

