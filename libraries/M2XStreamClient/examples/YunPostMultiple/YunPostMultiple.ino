#include <Console.h>
#include <YunClient.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

char deviceId[] = "<device id>"; // Device you want to push to
char m2xKey[] = "<M2X access key>"; // Your M2X access key

char incomingByte;      // a variable to read incoming Console data into

// Values to push, for simplicity, we just use predefined data here
const char *streamNames[] = { "temperature", "humidity" };
int counts[] = { 2, 1 };
const char *ats[] = { "2013-09-09T19:15:00Z",
                      "2013-09-09T19:15:10Z",
                      "2013-09-09T19:15:20Z"};
double values[] = { 23.75, 24.125, 6.375 };

YunClient client;
M2XStreamClient m2xClient(&client, m2xKey);

void setup() {
  Bridge.begin();   // Initialize Bridge
  Console.begin();  // Initialize Console

  // Wait for the Console port to connect
  while(!Console);

  Console.println("type T to push multiple stream data to m2x");

}

void loop() {
  // see if there's incoming Console data:
  if (Console.available() > 0) {
    // read the oldest byte in the Console buffer:
    incomingByte = Console.read();
    Console.println(incomingByte);

    // if the user presses 'T', push the data to m2x
    if (incomingByte == 'T') {
      int response = m2xClient.postDeviceUpdates(deviceId, 2, streamNames,
                                                 counts, ats, values);
      Console.print("M2x client response code: ");
      Console.println(response);
    }
  }
}
