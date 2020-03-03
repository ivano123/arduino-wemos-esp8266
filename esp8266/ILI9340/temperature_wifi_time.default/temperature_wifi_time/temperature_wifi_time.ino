const char ssid[] = "MikroTik-191AD2";
const char pass[] = "zebra222";


// define time
#include <TimeLib.h>
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <CoinMarketCapApi.h>


const char* URL = "http://api.fixer.io/latest?base=USD&symbols=JPY";  // http resource
//sample json data used in this sketch
// {"base":"USD","date":"2017-11-03","rates":{"JPY":113.94}}


static const char ntpServerName[] = "us.pool.ntp.org";
const int timeZone = 3;
WiFiUDP Udp;
unsigned int localPort = 8888;


#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h" // https://github.com/glennirwin/Adafruit_ILI9340
#include <DallasTemperature.h> //Siehe Hinweis oben, verwendet wird https://github.com/milesburton/Arduino-Temperature-Control-Library
//#include <Base64.h>
#include <OneWire.h>

#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

WiFiClientSecure client;
CoinMarketCapApi api(client);

char temperaturStr[6];
float merketemperatur=0;

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);


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
 Serial.begin(9600);
 // Dallas sensor init
 DS18B20.begin();

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
Serial.print("day of week");
Serial.print(weekday());


// digitalClockDisplay();
 
 tft.begin();
 tft.setRotation( ROTATION_270);
 tft_zeichneRahmen();
 //tft_zeichneBeschriftung();
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

 /*tft.setTextSize(2); 
 tft.setCursor( 7,15);
 tft.print("github.com/shaposhnikoff");
 tft.setTextSize(2); 
 tft.setCursor( 10,210);
 tft.print("Max I. Shaposhnikoff");
 */
 
}

void tft_zeichneRahmen( )
{
 tft.fillScreen(ILI9340_BLACK);
 //tft.drawRoundRect(1, 0, tft.width()-1,40, 10, ILI9340_BLUE);
 //tft.drawRoundRect(1, 50, tft.width()-1,135, 10, ILI9340_GREEN);
// tft.drawRoundRect(0, 200, tft.width()-1, 40, 10, ILI9340_RED);
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
 Serial.println("*");
 Serial.println(temperatur);
 Serial.println("*");
 Serial.println(temperaturStr); 
 Serial.print(day());
 
 digitalClockDisplay();
 /*
 tft.setTextSize(2); 
 tft.setTextColor(ILI9340_YELLOW, ILI9340_BLACK ); 
 tft.setCursor(25,90);
 tft.print(temperaturStr);
 //tft.fillScreen(ILI9340_WHITE); 
 tft.setCursor(30,120);
 tft.print(day());
 tft.setCursor(40,120);
 tft.print(".");
 tft.setCursor(50,120);
 tft.print(month());
 tft.setCursor(60,120);
 tft.print(".");
 tft.print(year());
 tft.setCursor(60,140);
 tft.print(hour());
 tft.setCursor(90,140);
 tft.print(minute());
 tft.setCursor(120,140);
 tft.print(second());
 
 */




 
 //tft.setCursor(20,160);


 
}

void loop(void) 
{
 delay(500); 
 float temperatur = getTemperatur();
 
/* if (merketemperatur!=temperatur) {
 zeigeTemperatur(temperatur);
 digitalClockDisplay();
 merketemperatur=temperatur;
 }

*/


get_exchange_rate();

printTickerData("bitcoin");
printTickerData("ethereum");
printTickerData("dogecoin");


// example start 

tft.setTextColor(ILI9340_CYAN, ILI9340_BLACK);
tft.setTextSize(2);
tft.setCursor(15, 15);
tft.print("Date");
tft.setCursor(115, 15);

// day
tft.setCursor(180, 15);
if(day()<10){tft.print("0");}        
tft.print(day());

/*
tft.setCursor(215, 15);
tft.print(".");
*/

tft.setCursor(140, 15);
if(weekday()==1){tft.print("Sun");}
if(weekday()==2){tft.print("Mon");}
if(weekday()==3){tft.print("Thu");}
if(weekday()==4){tft.print("Wen");}
if(weekday()==5){tft.print("The");}
if(weekday()==6){tft.print("Fri");}
if(weekday()==7){tft.print("Sat");}


//tft.fillScreen(ILI9340_BLACK); 

// month
tft.setCursor(214, 15);
//if(month()<10){tft.print("0");}        
//tft.print(month());

if(month()==1){tft.print("Jan");}
if(month()==2){tft.print("Feb");}
if(month()==3){tft.print("Mar");}
if(month()==4){tft.print("Apr");}
if(month()==5){tft.print("May");}
if(month()==6){tft.print("Jun");}
if(month()==7){tft.print("Jul");}
if(month()==8){tft.print("Aug");}
if(month()==9){tft.print("Sep");}
if(month()==10){tft.print("Okt");}
if(month()==11){tft.print("Nov");}
if(month()==12){tft.print("Dec");}


//tft.fillScreen(ILI9340_BLACK); 
//year
tft.setCursor(260, 15);
tft.print(year()); // year

tft.setTextSize(3);
tft.setCursor(35, 50);
//tft.print("ARDUINO CLOCK");
        ///////////////////////// TIME //////////////////////
tft.setCursor(35, 100);
tft.setTextSize(5);
       // print hour
        
if(hour()<10){tft.print("0");}
        
tft.print(hour());
tft.print(":");
tft.setCursor(125, 100);
        
        // print minute
if(minute()<10){tft.print("0");}
tft.print(minute());
tft.print(":");


tft.setCursor(215, 100);
if(second()<10){tft.print("0");}
tft.print(second());

        //////////////////////// WEEK DAY ////////////////////
tft.setTextSize(3);
tft.setCursor(80, 150);
        
tft.setTextSize(1);
tft.setCursor(100, 197);
tft.print("www.shaposhnikoff.info");


tft.setCursor(30, 215); //Go to last line of the LCD Screen
tft.setTextSize(2);
tft.print("Temperature: ");
tft.setCursor(180, 215); 
tft.print(temperatur);
tft.print(" C");



// example end       

delay(1000);                  // waits for a second

tft.fillScreen(ILI9340_BLACK);

tft.setTextSize(3);
tft.setCursor(35, 50);
tft.print("ARDUINO CLOCK");

delay(2000);                  // waits for a second
tft.fillScreen(ILI9340_BLACK);


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





void printTickerData(String ticker) {
 // tft.print("Getting ticker data for " + ticker");
  
  //Serial.println("---------------------------------");
  //Serial.println("Getting ticker data for " + ticker);


  // Ticker unfortunately is not the symbol for some reason.
  // Go to CoinMarketCap.com and select the coin you would like to check
  // The ticker name makes up the last part of the URL
  // e.g: http://coinmarketcap.com/currencies/bitcoin/ , "bitcoin" is the ticker value

  // Currency is optional, so you can pass only ticker if you want.
  // Check out the currency drop down on CoinMarketCap.com to get available values
  CMCTickerResponse response = api.GetTickerInfo(ticker, "eur");
  if (response.error == "") {


    
    tft.fillScreen(ILI9340_BLACK);
    tft.setTextSize(4);
    tft.setCursor(100, 30);
    tft.print(response.symbol);
  
    //tft.setCursor(20, 70);
    //tft.print("Rank: ");
    //tft.setCursor(100, 70);
    //tft.print(response.rank);
   // tft.setCursor(20, 90);
   // tft.print("Price in USD: ");
   tft.setTextSize(5);
    tft.setCursor(100, 90);
    tft.print(response.price_usd);
    delay(5000);
    tft.fillScreen(ILI9340_BLACK);
    /*
    Serial.print("Price in BTC: ");
    Serial.println(response.price_btc);

    Serial.print("24h Volume USD: ");
    Serial.println(response.volume_usd_24h);
    Serial.print("Market Cap USD: ");
    Serial.println(response.market_cap_usd);

    Serial.print("Available Supply: ");
    Serial.println(response.available_supply);
    Serial.print("Total Supply: ");
    Serial.println(response.total_supply);

    Serial.print("Percent Change 1h: ");
    Serial.println(response.percent_change_1h);
    Serial.print("Percent Change 24h: ");
    Serial.println(response.percent_change_24h);
    Serial.print("Percent Change 7d: ");
    Serial.println(response.percent_change_7d);
    Serial.print("Last Updated: ");
    Serial.println(response.last_updated);

    // These fields will not come back if you do not request a currency
    Serial.print("Price in requested currecy: ");
    Serial.println(response.price_currency);
    Serial.print("24h Volume in requested currency: ");
    Serial.println(response.volume_currency_24h);
    Serial.print("Market Cap in requested currency: ");
    Serial.println(response.market_cap_currency);

*/
  } else {
    Serial.print("Error getting data: ");
    Serial.println(response.error);
  }
  Serial.println("---------------------------------");
}





void get_exchange_rate() {
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(1);
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  // configure targed server and url
  http.begin(URL);
 
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  if(httpCode != HTTP_CODE_OK) {
    return;
  }
  String buffer = http.getString();
  Serial.println(buffer);
  //parse json data
  char json[buffer.length() + 1];
  buffer.toCharArray(json, sizeof(json));
  Serial.println(json);
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  const char* date = root["date"];
  Serial.println(date);
  const char* base = root["base"];
  Serial.println(base);
  JsonObject& rates = root["rates"];
  rates.printTo(Serial);
  Serial.println();
  const char* rate = rates["JPY"];
  Serial.println(rate);
  Serial.println();
  
  Serial.println("closing connection");
  //OLED.clearDisplay();
  //OLED.setCursor(0,0);
  // Print the IP address
  //OLED.print("http://");
  tft.setTextSize(2);
  tft.print(WiFi.localIP());
  tft.println("/");
  tft.setCursor(0, 26);
  tft.print(date);
  tft.setCursor(0, 54);
  tft.print("JPY/");
  tft.print(base);
  tft.print(": ");
  tft.print(rate);
  delay(3000);
  //OLED.display(); //output 'display buffer' to screen  
}




