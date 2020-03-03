#include <SPI.h>
#include <WiFi.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

char ssid[] = "<ssid>"; //  your network SSID (name)
char pass[] = "<WPA password>";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

char deviceId[] = "<device id>"; // Device you want to push to
char m2xKey[] = "<M2X access key>"; // Your M2X access key

const char *streamNames[] = { "temperature", "humidity" };
int counts[] = { 2, 1 };
const char *ats[] = { "2013-09-09T19:15:00Z",
                      "2013-09-09T19:15:10Z",
                      "2013-09-09T19:15:20Z"};
double values[] = { 10.0, 20.0, 7.5 };

WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey,idler);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {
  int response = m2xClient.postDeviceUpdates(deviceId, 2, streamNames,
                                             counts, ats, values);
  Serial.print("M2x client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}

void idler(void)
// Do something while M2X is waiting reply to postDeviceUpdates.
// If your project is very time critical, it must have a possiblity
// do some checks during network io. 
// This function only prints char I in serial terminal to demonstrate the feature.
{
  Serial.print('I');
  delay(50);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}
