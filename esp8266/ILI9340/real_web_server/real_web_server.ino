/*
 NodeMCU-Server-TFT-Temperatur

 NodeMCU fungiert als Server und empfängt Temperatur per URL
 Temperaturwerte (z.B. Außentemperatur) wird auf angeschlossenem 
 QVGA-Display ausgegeben,
 
 Parameteruebergabe-Syntax:
 http://192.168.2.75/sensor/temperatur/?pw=passwortxyz&idnr=1&wert=20
 
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
#include "Adafruit_ILI9340.h" //verwendete Lib für Display https://github.com/glennirwin/Adafruit_ILI9340
#include <DallasTemperature.h> //Siehe Hinweis oben, verwendet wird https://github.com/milesburton/Arduino-Temperature-Control-Library
//#include <Base64.h>
#include <OneWire.h>

#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float merketemperatur=0;
float merkeaussentemperatur=0;

//Pinbelegung für Display

#define TFT_CS D0 //GPIO 16 interne LED kein Port nötig, da TFT CS an GND
#define TFT_RST 10 //D12 GPIO10 //kein Port nötig, da Reset an Reset angeschlossen
#define TFT_DC D1 // GPIO 5
#define TFT_MOSI D7 // GPIO 13
#define TFT_CLK D5 // GPIO 14
#define TFT_MISO D6 // GPIO 12

Adafruit_ILI9340 tft = Adafruit_ILI9340(TFT_CS, TFT_DC, TFT_RST);

//Falls die Anzeige gedreht wird
#define ROTATION_NO 0
#define ROTATION_90 1
#define ROTATION_180 2
#define ROTATION_270 3

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "MikroTik-191AD3";
const char *pass = "zebra222";


//const char* ssid = "DEINE_SSID"; //Ihr Wlan,Netz SSID eintragen
//const char* pass = "DEIN_WLANPASSWORT"; //Ihr Wlan,Netz Passwort eintragen
IPAddress ip(192,168,1,75); //Feste IP des neuen Servers, frei wählbar
IPAddress gateway(192,168,1,1); //Gatway (IP Router eintragen)
IPAddress subnet(255,255,255,0); //Subnet Maske eintragen

ESP8266WebServer server(80);

void handleRoot() {
 //Temperatur auch bei Url-Aufruf zurückgeben
 String message="*** Ueberwachungs Server - Beispiel von www.mikrocontroller-elektronik.de ***\n";
 String tempstr= String(merketemperatur, 2); 
 message += "Temperatur Innen : " + tempstr +"\n";
 tempstr= String(merkeaussentemperatur, 2); 
 message += "Temperatur Aussen: " + tempstr +"\n";
 server.send(200, "text/plain", message);
}


void handleTemperatur() {
 //printUrlArg(); //fuer Debugz Zwecke

 String stemp =server.arg("wert");
 float temperatur = stemp.toFloat();
 if (merkeaussentemperatur!=temperatur) {
 zeigeAussenTemperatur(temperatur);
 merkeaussentemperatur=temperatur;
 }

 //Temperatur auch bei Url-Aufruf zurückgeben
 String message="*** Ueberwachungs Server - Beispiel von www.mikrocontroller-elektronik.de ***\n";
 String tempstr= String(merketemperatur, 2); 
 message += "Temperatur Innen : " + tempstr +"\n";
 tempstr= String(merkeaussentemperatur, 2); 
 message += "Temperatur Aussen: " + tempstr +"\n";
 server.send(200, "text/plain", message);
}


void printUrlArg() {
 //Alle Parameter seriell ausgeben
 String message="";
 for (uint8_t i=0; i<server.args(); i++){
 message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
 }
 server.send(200, "text/plain", message);
 Serial.println(message);
}

 
void setup() {

 DS18B20.begin(); //lokale Temperaturmessung
 
 Serial.begin(115200);
 Serial.println(" \r\nINIT \r\n") ;

 WiFi.begin(ssid, pass);
 WiFi.config(ip, gateway, subnet);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 Serial.print("Verbunden mit ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 
 server.on("/",handleRoot) ;
 server.on("/sensor/temperatur/", handleTemperatur);
 server.begin();
 
 Serial.println("HTTP Server wurde gestartet!");

 tft.begin();
 tft.setRotation( ROTATION_270);
 tft_zeichneRahmen();
 tft_zeichneBeschriftung();
 
}


void tft_zeichneBeschriftung(void)
{
 tft.setTextColor(ILI9340_WHITE); 
 tft.setCursor( 10,5);
 tft.setTextSize(2); 
 tft.print("Innentemperatur");

 tft.setCursor( 285,5);
 tft.setTextSize(1); 
 tft.print("O");
 tft.setTextSize(3); 
 tft.print("C");


 tft.setTextColor(ILI9340_WHITE); 
 tft.setCursor( 10,127);
 tft.setTextSize(2); 
 tft.print("Aussentemperatur");

 tft.setCursor( 285,127);
 tft.setTextSize(1); 
 tft.print("O");
 tft.setTextSize(3); 
 tft.print("C");
}


void tft_zeichneRahmen( )
{
 
 tft.fillScreen(ILI9340_BLACK);
 
 tft.drawRoundRect(0, 0, tft.width()-1,117, 10, ILI9340_GREEN);
 tft.drawRoundRect(0, 122, tft.width()-1,117, 10, ILI9340_BLUE); 
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
 char temperaturStr[6];
 dtostrf(temperatur, 2, 1, temperaturStr);
 Serial.print("Temperatur: "); 
 Serial.println(temperaturStr); 

 tft.setTextSize(10); 
 tft.setTextColor(ILI9340_MAGENTA, ILI9340_BLACK ); 
 tft.setCursor(20,30);
 tft.print(temperaturStr);
 
}

void zeigeAussenTemperatur(float temperatur)
{
 char temperaturStr[6];
 dtostrf(temperatur, 2, 1, temperaturStr);
 Serial.print("AussenTemperatur: "); 
 Serial.println(temperaturStr); 
 
 tft.setTextSize(10); 
 tft.setTextColor(ILI9340_YELLOW, ILI9340_BLACK ); 
 tft.setCursor(20,152);
 tft.print(temperaturStr);
 
}

 
void loop(void)
{
 server.handleClient();
 
 delay(500); 

 float temperatur = getTemperatur();
 if (merketemperatur!=temperatur) {
 zeigeTemperatur(temperatur);
 merketemperatur=temperatur;
 }
}
