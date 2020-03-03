#include <ESP8266WiFi.h>
#include <AmazonIOTClient.h>
#include "ESP8266AWSImplementations.h"

Esp8266HttpClient httpClient;
Esp8266DateTimeProvider dateTimeProvider;

AmazonIOTClient iotClient;
ActionError actionError;

char *ssid="MikroTik-191AD2";
char *password="zebra222";



void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WAP
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  iotClient.setAWSRegion("us-east-1");
  iotClient.setAWSEndpoint("amazonaws.com");
  iotClient.setAWSDomain("a2czqguov0oe41-ats.iot.us-east-1.amazonaws.com");
  iotClient.setAWSPath("/things/Thing1/shadow");
  iotClient.setAWSKeyID("AKIASGKN4QIA44FZ3BDF");
  iotClient.setAWSSecretKey("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");
  iotClient.setHttpClient(&httpClient);
  iotClient.setDateTimeProvider(&dateTimeProvider);
}

void loop(){
  char* shadow = "{\"state\":{\"reported\": {\"foobar\": \"bar\"}}}";

  char* result = iotClient.update_shadow(shadow, actionError);
  Serial.print(result);

  delay(60000);
}
