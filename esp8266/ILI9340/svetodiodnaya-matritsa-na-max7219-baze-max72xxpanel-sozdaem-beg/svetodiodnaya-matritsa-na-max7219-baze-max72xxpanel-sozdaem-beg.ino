#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <time.h>


/*#include <WiFiClientSecure.h>*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bmp; 


// define time 
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

static const char ntpServerName[] = "us.pool.ntp.org";
const int timeZone = 3;     
WiFiUDP Udp;
unsigned int localPort = 8888;  


const char ssid[] = "MikroTik-191AD2";
const char pass[] = "zebra222";       

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);


// DHT22 
#include "DHT.h"
#define DHTTYPE DHT22
#define DHTPIN D3
DHT dht(DHTPIN, DHTTYPE);

/* Led matrix settings */
int pinCS = 2; 
int numberOfHorizontalDisplays = 5;
int numberOfVerticalDisplays = 1;


Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "";
int wait = 20; // время в миллисекундах
int spacer = 2; // Промежуток между символами (кол-во точек)
int width = 5 + spacer; // Ширина шрифта 5 пикселей



//*ПЕРЕМЕНЫЕ
//**********************************
int stroka = 0;//переменая строки
String peremenay1 = "20 ";
String peremenay2 = "75 ";
String peremenay3 = "24 ";
String peremenay4 = "variable 4";
//**********************************

void setup() {
  Serial.begin(9600);
  matrix.setIntensity(8); // Используйте значение между 0 и 15 для яркости
  matrix.setRotation(matrix.getRotation()+0); //развернуть строку 1 - 90, 2 - 180, 3 - 270

  Serial.println(F("BMP280 test"));
   if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!")); while (1); 
    }

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
    Serial.print(".");
    Serial.print(".");
        
  }
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

 
}


time_t prevDisplay = 0; // when the digital clock was displayed


void loop() {

  matrix.fillScreen(LOW);

  //started time show code

   
  
  // сдесь в зависимости от перменых на матрицу выводится нужная строка
  //if (stroka == 0) {tape = utf8rus(" first string ,"+peremenay1+peremenay4+" text"+peremenay3+"text21 "+peremenay2+"%");}
  String mesyc = " ";
  
/*  if (stroka == 0) {tape = ("first sensor- BMP280");}
  if (stroka == 1) {tape = (mesyc +"BMP280 temperature, "+bmp.readTemperature() +"`C, BMP280 Pressure, "+bmp.readPressure()+" Pas. "+" BMP280 Altitude, "+bmp.readAltitude()+" m.");}
  if (stroka == 2) {tape = ("second sensor - DHT22");}
  if (stroka == 3) {tape = (mesyc + " DHT22 Humidity "+dht.readHumidity());}
  if (stroka == 4) {tape = (mesyc + " DHT22 Temperature "+dht.readTemperature());}
  if (stroka == 5) {tape = ("NTP Date and time");}
  if (stroka == 6) {tape = (mesyc + "Time is " + hour()+"h"+minute()+"m"+second()+"s");}
  if (stroka == 7) {tape = (mesyc + "Date is " + day()+"."+month()+"."+year()+"");}
 */

if (stroka == 0) {tape = (mesyc +dht.readHumidity()+" %");}
if (stroka == 1) {tape = (mesyc +dht.readTemperature()+" C");}
if (stroka == 2) {tape = (mesyc + hour()+":"+minute()+":"+second());}
    
  //if (stroka == 1) {tape = utf8rus("second string ,"+peremenay1+peremenay4+" text"+peremenay3+"text22 "+peremenay2+"%");}
  //if (stroka == 2) {tape = utf8rus("third string ,"+peremenay1+peremenay4+" text"+peremenay3+"text23 "+peremenay2+"%");}
  //if (stroka== 3) {tape = utf8rus("four string ,"+peremenay1+peremenay4+" text"+peremenay3+"text24 "+peremenay2+"%");}
  //if (stroka == 4) {tape = utf8rus("five string ,"+peremenay1+peremenay4+" text"+peremenay3+"text25 "+peremenay2+"%");}

stroka += 1;
if (stroka == 3) {stroka = 0;}
  
 for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) 
       {
    matrix.fillScreen(LOW);
 
    int letter = i / width; // номер символа выводимого на матрицу 
    int x = (matrix.width() - 1) - i % width;  
    int y = (matrix.height() - 8) / 2; // отцентрировать текст по вертикали

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length()) 
      { matrix.drawChar(x, y, tape[letter], HIGH, LOW,1); }
      letter--;
      x -= width;
    }
   
    matrix.write(); // Отправить картинку для отображения
    delay(wait);
  }

  Serial.println("***************************");

  digitalClockDisplay();
  Serial.print (F("Dht22 temp. = "));
  Serial.print (dht.readTemperature());
  Serial.println(" *C");
  
  Serial.print (F("Dht22 Hum. = "));
  Serial.print (dht.readHumidity());
  Serial.println(" *%");
    
  Serial.print (F("Bmp280 temp = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("BMP280 Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("BMP280 Approx altitude = "));
  Serial.print(bmp.readAltitude(1020)); // this should be adjusted to your local forcase
  Serial.println(" m");

  Serial.println();
  
  delay(20);



}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print("Current date and time ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}


void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void display_message(String message){
   for ( int i = 0 ; i < width * message.length() + matrix.width() - spacer; i++ ) {
    matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < message.length() ) {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1); // HIGH LOW means foreground ON, background off, reverse to invert the image
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display
    delay(wait/2);
  }
}




















