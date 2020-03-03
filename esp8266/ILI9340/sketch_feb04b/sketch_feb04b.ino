/*
 NodeMCU-TFT-Temperatur
 
 TFT-Display mit Controller ILI9341 am NodeMCU-Board
 Verwendetes Display: 2.2 Zoll Serial TFT-Farbdisplay; SPI; Auflösung 240x320 Pixel 
 Bezugsquellen Display:
 Amazon - http://amzn.to/2iNmaXx / Amazon - http://amzn.to/2j1VIXK
 
 Temperatur wird über angeschlossenen Sensor DS18B20 ausgelesen und auf dem Display angezeigt
 Temperatursensor DS18B20 an Pin D2 
 Bezugsquelle Temperatursensor: Reichelt / Conrad / Amazon - http://amzn.to/2i3WlRX 
 
 Notwendig ist die angepasste Display-Lib: https://github.com/glennirwin/Adafruit_ILI9340
 
 Programm erprobt ab Arduino IDE 1.6.13
 Projektbeschreibung und weitere Beispiele unter http://www.mikrocontroller-elektronik.de/
*/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h" // https://github.com/glennirwin/Adafruit_ILI9340
#include <DallasTemperature.h> //Siehe Hinweis oben, verwendet wird https://github.com/milesburton/Arduino-Temperature-Control-Library
//#include <Base64.h>
#include <OneWire.h>

#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

char temperaturStr[6];
float merketemperatur=0;

//Pinbelegung für Display

#define TFT_CS D0   // GPIO 16 interne LED kein Port nötig, da TFT CS an GND
#define TFT_RST 10  // D12 GPIO10 //kein Port nötig, da Reset an Reset angeschlossen
#define TFT_DC D1   // GPIO 5
#define TFT_MOSI D7 // GPIO 13
#define TFT_CLK D5  // GPIO 14
#define TFT_MISO D6 // GPIO 12

Adafruit_ILI9340 tft = Adafruit_ILI9340(TFT_CS, TFT_DC, TFT_RST);

//Falls die Anzeige gedreht wird
#define ROTATION_NO 0
#define ROTATION_90 1
#define ROTATION_180 2
#define ROTATION_270 3

void setup() 
{
 DS18B20.begin();

 Serial.begin(9600);
 
 tft.begin();
 
 tft.setRotation( ROTATION_270);
 tft_zeichneRahmen();
 tft_zeichneBeschriftung();
}


void tft_zeichneBeschriftung(void)
{
 tft.setTextColor(ILI9340_WHITE); 
 tft.setCursor( 10,55);
 tft.setTextSize(2); 
 tft.print("Temperature in ");
 tft.setTextSize(1); 
 tft.print("O");
 tft.setTextSize(3); 
 tft.print("C");

 tft.setTextSize(2); 
 tft.setCursor( 7,15);
 tft.print("github.com/shaposhnikoff");
 tft.setTextSize(2); 
 tft.setCursor( 10,210);
 tft.print("Max I. Shaposhnikoff");
}

void tft_zeichneRahmen( )
{
 tft.fillScreen(ILI9340_BLACK);
 tft.drawRoundRect(1, 0, tft.width()-1,40, 10, ILI9340_BLUE);
 tft.drawRoundRect(1, 50, tft.width()-1,135, 10, ILI9340_GREEN);
 tft.drawRoundRect(0, 200, tft.width()-1, 40, 10, ILI9340_RED);
 }

float getTemperatur() {
 float temp;
 do {
 DS18B20.requestTemperatures(); 
 temp = DS18B20.getTempCByIndex(0);
 delay(100);
 } while (temp == 85.0 || temp == (-127.0));
 return temp;
}

void zeigeTemperatur(float temperatur)
{
 dtostrf(temperatur, 2, 1, temperaturStr);
 Serial.print("Temperatur: "); 
 Serial.println(temperaturStr); 
 tft.setTextSize(10); 
 tft.setTextColor(ILI9340_YELLOW, ILI9340_BLACK ); 
 tft.setCursor(20,90);
 tft.print(temperaturStr);
 
}

void loop(void) 
{
 delay(500); 
 float temperatur = getTemperatur();
 if (merketemperatur!=temperatur) {
 zeigeTemperatur(temperatur);
 merketemperatur=temperatur;
 }

}
