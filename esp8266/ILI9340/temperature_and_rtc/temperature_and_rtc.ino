/*
  DS3231: Real-Time Clock. Temperature example
  Read more: www.jarzebski.pl/arduino/komponenty/zegar-czasu-rzeczywistego-rtc-ds3231.html
  GIT: https://github.com/jarzebski/Arduino-DS3231
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
  www.MiniArduino.com
*/
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9340.h> // Hardware-specific library
#include <SPI.h>
#include <Wire.h>
#include <ds3231.h>
#define sclk 13  // Don't change
#define mosi 11  // Don't change
#define cs   9
#define dc   8
#define rst  7  // you can also connect this to the Arduino reset

//Adafruit_ILI9341_AS tft = Adafruit_ILI9341_AS(cs, dc, rst);       // Invoke custom library
ds3231 clock;
//RTCDateTime dt;

void setup()
{
  clock.begin();

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
}
void loop()
{

  // The temperature registers are updated after every 64-second conversion.
  // If you want force temperature conversion use forceConversion()
  //clock.forceConversion();
//  tft.setTextSize(4);
//  tft.setCursor(20, 10);
 // tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
/*  tft.print("Temperature");
  tft.setTextSize(2);
  tft.setCursor(10, 70);
  tft.setTextColor(0x39C4, ILI9341_BLACK);  // Leave a 7 segment ghost image, comment out next line!
  tft.drawString("88.88",10,70,7); // Overwrite the text to clear it
  tft.setTextColor(0x7E0); // Green
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
//  tft.print(clock.readTemperature());
//  tft.drawFloat(clock.readTemperature(), 5, 10, 70, 7);
  tft.setTextSize(1);
  tft.setCursor(95, 180);
  tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  tft.print("www.MiniArduino.com");
  tft.setTextSize(4);
  tft.setCursor(60, 200);
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.print("Celsius");*/
  delay(10000);
}
