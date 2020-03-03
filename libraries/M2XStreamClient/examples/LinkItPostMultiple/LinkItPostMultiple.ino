#include <LWiFi.h>
#include <LWiFiClient.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

char ssid[] = "<ssid>"; //  your network SSID (name)
char pass[] = "<WPA password>";    // your network password (use for WPA, or use as key for WEP)

char deviceId[] = "<device id>"; // Device you want to push to
char m2xKey[] = "<M2X access key>"; // Your M2X access key

const char *streamNames[] = { "temperature", "humidity" };
int counts[] = { 2, 1 };
const char *ats[] = { "2013-09-09T19:15:00Z",
                      "2013-09-09T19:15:10Z",
                      "2013-09-09T19:15:20Z"};
double values[] = { 10.0, 20.0, 7.5 };

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
  int response = m2xClient.postDeviceUpdates(deviceId, 2, streamNames,
                                             counts, ats, values);
  Serial.print("M2X client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}
