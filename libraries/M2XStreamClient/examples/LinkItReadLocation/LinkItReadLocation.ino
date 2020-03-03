#include <LWiFi.h>
#include <LWiFiClient.h>

#define ARDUINO_PLATFORM
#define M2X_ENABLE_READER
#include <jsonlite.h>
#include "M2XStreamClient.h"

char ssid[] = "<ssid>"; //  your network SSID (name)
char pass[] = "<WPA password>";    // your network password (use for WPA, or use as key for WEP)

char deviceId[] = "<device id>"; // Device you want to read
char m2xKey[] = "<M2X access key>"; // Your M2X access key

LWiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);

void on_location_found(const char* name,
                       double latitude,
                       double longitude,
                       double elevation,
                       const char* timestamp,
                       int index,
                       void* context) {
  Serial.print("Found a location, index:");
  Serial.println(index);
  Serial.print("Name: ");
  Serial.println(name);
  Serial.print("Latitude: ");
  Serial.println(latitude);
  Serial.print("Longitude: ");
  Serial.println(longitude);
  Serial.print("Elevation: ");
  Serial.println(elevation);
  Serial.print("Timestamp: ");
  Serial.println(timestamp);
}

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
  int response = m2xClient.readLocation(deviceId, on_location_found, NULL);
  Serial.print("M2x client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}
