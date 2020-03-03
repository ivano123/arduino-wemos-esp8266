#include <Adafruit_GFX.h>
#include <Adafruit_ILI9340.h>

#define tft_sclk 13
#define tft_miso 12
#define tft_mosi 11
#define tft_cs 10
#define tft_dc 9
#define tft_rst 8

Adafruit_ILI9340 tft = Adafruit_ILI9340(tft_cs, tft_dc,tft_rst);

void setup() {
  Serial.begin(115200);
   Serial.println("ILI9340 Test!");   
 tft.begin();
 tft.setRotation(1);
 tft.fillScreen(ILI9340_WHITE);
 tft.setTextColor(ILI9340_BLACK);
 tft.setTextSize(2);
 tft.setCursor(20,46); 
 tft.println("Hello World!"); 
}
void loop() {  
    while(1)
    {
      ;
    }
}

