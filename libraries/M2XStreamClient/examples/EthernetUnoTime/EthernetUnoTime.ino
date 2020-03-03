#include <SPI.h>
#include <Ethernet.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,17);

char* deviceId = "<device id>"; // Device you want to push to
char* streamName = "<stream name>"; // Stream you want to push to
char* m2xKey = "<M2X access key>"; // Your M2X access key

char timestamp[25];

const int temperaturePin = 0;
EthernetClient client;
M2XStreamClient m2xClient(&client, m2xKey);
TimeService timeService(&m2xClient);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.print("Attempting to connect to Ethernet");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  // wait 10 seconds for connection:
  delay(10000);

  Serial.println("Connected to Ethernet");
  printEthernetStatus();

  if (!m2x_status_is_success(timeService.init())) {
    Serial.println("Cannot initialize time service!");
    while(1) {}
  }
}

void loop() {
  float voltage, degreesC, degreesF;

  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;

  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.println(degreesF);

  int length = 25;
  timeService.getTimestamp(timestamp, &length);

  Serial.print("Current timestamp: ");
  Serial.println(timestamp);

  char *timestamps[1];
  timestamps[0] = timestamp;

  int count = 1;
  int response = m2xClient.postDeviceUpdates<float>(
      deviceId, 1, (const char **) &streamName,
      &count, (const char **) timestamps, &degreesC);
  Serial.print("M2x client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}

void printEthernetStatus() {
  // print local IP
  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());

  // print subnet mask
  Serial.print("Subnet Mask: ");
  Serial.println(Ethernet.subnetMask());

  // print gateway IP
  Serial.print("Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());

  // print dns IP
  Serial.print("DNS IP: ");
  Serial.println(Ethernet.dnsServerIP());
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}
