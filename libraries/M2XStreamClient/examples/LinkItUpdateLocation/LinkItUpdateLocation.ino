#include <LWiFi.h>
#include <LWiFiClient.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

char ssid[] = "<ssid>"; //  your network SSID (name)
char pass[] = "<WPA password>";    // your network password (use for WPA, or use as key for WEP)

char deviceId[] = "<device id>"; // Device you want to update
char m2xKey[] = "<M2X access key>"; // Your M2X access key

char name[] = "<location name>"; // Name of current location of datasource
double latitude = -37.97884;
double longitude = -57.54787; // You can also read those values from a GPS
double elevation = 15;

LWiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);

void setup() {
  Serial.begin(9600);
  while (!Serial.available()) {}
  while (Serial.available()) { Serial.read(); }
  LWiFi.begin();

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  if (LWiFi.connectWPA(ssid, pass) < 0) {
    Serial.println("Failed to connect to Wifi!");
    /* Loop forever */
    while (1) ;
  }
  Serial.println("Connected to wifi");
}

void loop() {
  int response = m2xClient.updateLocation(deviceId, name, latitude, longitude,
                                          elevation);
  Serial.print("M2x client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}
