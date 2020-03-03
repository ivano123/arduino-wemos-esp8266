#include <LWiFi.h>
#include <LWiFiClient.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

char ssid[] = "<ssid>"; //  your network SSID (name)
char pass[] = "<WPA password>";    // your network password (use for WPA, or use as key for WEP)

char deviceId[] = "<device id>"; // Device you want to post to
char streamName[] = "<stream name>"; // Stream you want to post to
char m2xKey[] = "<M2X access key>"; // Your M2X access key

char fromTime[] = "<from date/time>"; // yyyy-mm-ddTHH:MM:SS.SSSZ
char endTime[] = "<end date/time>"; // yyyy-mm-ddTHH:MM:SS.SSSZ

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

  // Delete values
  int response = m2xClient.deleteValues(deviceId,
                                        streamName,
                                        fromTime,
                                        endTime);
  Serial.print("M2x client response code: ");
  Serial.println(response);
}

void loop() {
  // Do nothing
  delay(5000);
}
