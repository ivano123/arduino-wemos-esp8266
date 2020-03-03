#include <Console.h>
#include <YunClient.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

char* deviceId = "<device id>"; // Device you want to push to
char* streamName = "<stream name>"; // Stream you want to push to
char* m2xKey = "<M2X access key>"; // Your M2X access key

char incomingByte;      // a variable to read incoming Console data into
const int sensorPin = A0;

char timestamp[25];

YunClient client;
M2XStreamClient m2xClient(&client, m2xKey);
TimeService timeService(&m2xClient);

void setup() {
  Bridge.begin();   // Initialize Bridge
  Console.begin();  // Initialize Console

  // Wait for the Console port to connect
  while(!Console);

  if (!m2x_status_is_success(timeService.init())) {
    Console.println("Cannot initialize time service!");
    while(1) {}
  }

  Console.println("type T to push temperature data to m2x");
}

void loop() {
  // see if there's incoming Console data:
  if (Console.available() > 0) {
    // read the oldest byte in the Console buffer:
    incomingByte = Console.read();
    Console.println(incomingByte);

    // if the user presses 'T', push the temperature to m2x
    if (incomingByte == 'T') {

      int sensorVal = analogRead(sensorPin);
      Console.print(F("sensor Value: "));
      Console.print(sensorVal);

      // convert the ADC reading to voltage
      float voltage = (sensorVal/1024.0) * 5.0;

      // Send the voltage level out the Serial port
      Console.print(F(", Volts: "));
      Console.print(voltage);

      // convert the voltage to temperature in degrees C
      // the sensor changes 10 mV per degree
      // the datasheet says there's a 500 mV offset
      // ((volatge - 500mV) times 100)
      Console.print(F(", degrees C: "));
      float temperature = (voltage - .5) * 100;
      Console.println(temperature);

      int length = 25;
      timeService.getTimestamp(timestamp, &length);

      Console.print(F("Current timestamp: "));
      Console.println(timestamp);

      char *timestamps[1];
      timestamps[0] = timestamp;

      int count = 1;
      int response = m2xClient.postDeviceUpdates<float>(
          deviceId, 1, (const char **) &streamName,
          &count, (const char **) timestamps, &temperature);
      Console.print("M2x client response code: ");
      Console.println(response);
    }
  }
}
