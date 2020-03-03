#include <Console.h>
#include <YunClient.h>

#define ARDUINO_PLATFORM
#define M2X_ENABLE_READER
#include <jsonlite.h>
#include "M2XStreamClient.h"

char deviceId[] = "<device id>"; // Device you want to read locations from
char m2xKey[] = "<M2X access key>"; // Your M2X access key

char incomingByte;      // a variable to read incoming Console data into

YunClient client;
M2XStreamClient m2xClient(&client, m2xKey);

void on_location_found(const char* name,
                       double latitude,
                       double longitude,
                       double elevation,
                       const char* timestamp,
                       int index,
                       void* context) {
  Console.print("Found a location, index:");
  Console.println(index);
  Console.print("Name: ");
  Console.println(name);
  Console.print("Latitude: ");
  Console.println(latitude);
  Console.print("Longitude: ");
  Console.println(longitude);
  Console.print("Elevation: ");
  Console.println(elevation);
  Console.print("Timestamp: ");
  Console.println(timestamp);
}

void setup() {
  Bridge.begin();   // Initialize Bridge
  Console.begin();  // Initialize Console

  // Wait for the Console port to connect
  while(!Console);

  Console.println("type T to read locations from m2x");
}

void loop() {
  // see if there's incoming Console data:
  if (Console.available() > 0) {
    // read the oldest byte in the Console buffer:
    incomingByte = Console.read();
    Console.println(incomingByte);

    // if the user presses 'T', reads the location
    if (incomingByte == 'T') {
      int response = m2xClient.readLocation(deviceId, on_location_found, NULL);
      Console.print("M2x client response code: ");
      Console.println(response);
    }
  }
}
