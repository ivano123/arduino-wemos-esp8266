/****************************
The MIT License (MIT)

Copyright (c) 2015 by bbx10node@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************/

#include <ESP8266WiFi.h>
#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#endif
#ifndef max
#define max(x,y) (((x)>(y))?(x):(y))
#endif
#include <ArduinoJson.h>

//const char SSID[]     = "********";
//const char PASSWORD[] = "***************";

const char SSID[] = "MikroTik-191AD3";
const char PASSWORD[] = "zebra222";






// Use your own API key by signing up for a free developer account.
// http://www.wunderground.com/weather/api/
#define WU_API_KEY "4844e54ea9946841"

// Specify your favorite location one of these ways.
//#define WU_LOCATION "CA/HOLLYWOOD"

// US ZIP code
//#define WU_LOCATION ""
//#define WU_LOCATION "90210"

// Country and city
//#define WU_LOCATION "Australia/Sydney"
#define WU_LOCATION "Europe/Kiev"

// 5 minutes between update checks. The free developer account has a limit
// on the  number of calls so don't go wild.
#define DELAY_NORMAL    (5*60*1000)
// 20 minute delay between updates after an error
#define DELAY_ERROR     (20*60*1000)

#define WUNDERGROUND "api.wunderground.com"

// HTTP request
const char WUNDERGROUND_REQ[] =
    "GET /api/" WU_API_KEY "/conditions/q/" WU_LOCATION ".json HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " WUNDERGROUND "\r\n"
    "Connection: close\r\n"
    "\r\n";

void setup()
{
  Serial.begin(9600);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

static char respBuf[4096];

void loop()
{
  // TODO check for disconnect from AP

  // Open socket to WU server port 80
  Serial.print(F("Connecting to "));
  Serial.println(WUNDERGROUND);

  // Use WiFiClient class to create TCP connections
  WiFiClient httpclient;
  const int httpPort = 80;
  if (!httpclient.connect(WUNDERGROUND, httpPort)) {
    Serial.println(F("connection failed"));
    delay(DELAY_ERROR);
    return;
  }

  // This will send the http request to the server
  Serial.print(WUNDERGROUND_REQ);
  httpclient.print(WUNDERGROUND_REQ);
  httpclient.flush();

  // Collect http response headers and content from Weather Underground
  // HTTP headers are discarded.
  // The content is formatted in JSON and is left in respBuf.
  int respLen = 0;
  bool skip_headers = true;
  while (httpclient.connected() || httpclient.available()) {
    if (skip_headers) {
      String aLine = httpclient.readStringUntil('\n');
      //Serial.println(aLine);
      // Blank line denotes end of headers
      if (aLine.length() <= 1) {
        skip_headers = false;
      }
    }
    else {
      int bytesIn;
      bytesIn = httpclient.read((uint8_t *)&respBuf[respLen], sizeof(respBuf) - respLen);
      Serial.print(F("bytesIn ")); Serial.println(bytesIn);
      if (bytesIn > 0) {
        respLen += bytesIn;
        if (respLen > sizeof(respBuf)) respLen = sizeof(respBuf);
      }
      else if (bytesIn < 0) {
        Serial.print(F("read error "));
        Serial.println(bytesIn);
      }
    }
    delay(1);
  }
  httpclient.stop();

  if (respLen >= sizeof(respBuf)) {
    Serial.print(F("respBuf overflow "));
    Serial.println(respLen);
    delay(DELAY_ERROR);
    return;
  }
  // Terminate the C string
  respBuf[respLen++] = '\0';
  Serial.print(F("respLen "));
  Serial.println(respLen);
  //Serial.println(respBuf);

  if (showWeather(respBuf)) {
    delay(DELAY_NORMAL);
  }
  else {
    delay(DELAY_ERROR);
  }
}

bool showWeather(char *json)
{
  StaticJsonBuffer<3*1024> jsonBuffer;

  // Skip characters until first '{' found
  // Ignore chunked length, if present
  char *jsonstart = strchr(json, '{');
  //Serial.print(F("jsonstart ")); Serial.println(jsonstart);
  if (jsonstart == NULL) {
    Serial.println(F("JSON data missing"));
    return false;
  }
  json = jsonstart;

  // Parse JSON
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println(F("jsonBuffer.parseObject() failed"));
    return false;
  }

  // Extract weather info from parsed JSON
  JsonObject& current = root["current_observation"];
  
  //const float temp_f = current["temp_f"];
  //Serial.print(temp_f, 1); Serial.print(F(" F, "));

  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");

  Serial.println("*******************************");
  const float temp_c = current["temp_c"];  Serial.print("Temperature: ");  Serial.println(temp_c, 1); 
  const float feelslike_c = current["feelslike_c"];  Serial.print("Temperature feels like: ");  Serial.println(feelslike_c, 1); 
   const char *humi = current[F("relative_humidity")];   Serial.print("relative humidity: ");        Serial.println(humi);  //Serial.println(F(" RH"));
  const char *weather = current["weather"];             Serial.print("Weather conditions is : ");   Serial.println(weather);   
  const char *wind_dir = current["wind_dir"];           Serial.print("Wind direction is : ");       Serial.println(wind_dir);  
  
  const char *wind_degrees = current["wind_degrees"];           Serial.print("Wind direction is : ");       Serial.println(wind_degrees);    
  const char *wind_kph = current["wind_kph"];           Serial.print("Wind speed is : ");       Serial.println(wind_kph); Serial.println("kph");   
  const char *pressure_mb = current["pressure_mb"];     Serial.print("Pressure is ");               Serial.println(pressure_mb);  
  const char *pressure_trend = current["pressure_trend"];     Serial.print("Pressure trend is ");               Serial.println(pressure_trend);  
  
  const char *obs_t = current["observation_time_rfc822"];  Serial.print("observation time is - ");  Serial.println(obs_t);  
  // Extract local timezone fields
  const char *local_tz_short = current["local_tz_short"];   Serial.println(local_tz_short);
  const char *local_tz_long = current["local_tz_long"];   Serial.println(local_tz_long);
  const char *local_tz_offset = current["local_tz_offset"];   Serial.println(local_tz_offset);

  
  
  
  
  
  
  
  
  return true;
}
