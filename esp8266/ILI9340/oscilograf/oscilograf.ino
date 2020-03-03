// UTFT_ViewFont (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This demo was made for serial modules with a screen resolution 
// of 220x176 pixels.
//
// This program requires the UTFT library.
//Firstly,you should install the UTFT library.
/***********************************************************************************
//-----------------Instructions  for Hardware IO Connection------------------------|
//-----TFT Pin---|----Arduino Pin-----| -------------------Note--------------------|
//------LED------|---------A0---------|---Backlight Control,Hight level Enable-----|
//------CLK------|---------A1---------|-----Serial Interface Clock Signal----------|
//------SDI------|---------A2---------|-----------Serial Input Signal--------------|
//------RS-------|---------A3---------|------Command or Parameter Sellect----------|
//------RST------|---------A4---------|---------------Reset Signal-----------------|
//------CS-------|---------A5---------|----------Chip Sellect Signal---------------|
//VCC:5V DC.
//GND:Ground.
//How to save IO pin(see the notes below):
//note1:LED  is also can be connected to 3.3V or 5V,So that Backlight will be always on.
//note2:RST can be connected to MCU reset pin,to save a IO pin.
//note3:CS  can be connected to GND,So that Chip Sellect will be always Enable.
************************************************************************************/
#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

//UTFT myGLCD(Model,SDA,SCL,CS,RST,RS)
//QD220A is for QDtech 2.2inch SPI LCD Module,Driver IC:ILI9225
UTFT myGLCD(QD220A,A2,A1,A5,A4,A3);   // Remember to change the model parameter to suit your display module!

int hours = 99;
int minutes = 0;

int count=0;

void setup()
{
myGLCD.InitLCD();
myGLCD.clrScr();
myGLCD.setFont(BigFont);
}

void displayDigits( int value , int x , int y ) {
String strValue = (value<10 ? "0" + String(value): String(value));
myGLCD.print(strValue , x, y );

}

void loop()
{
myGLCD.setFont(BigFont);
myGLCD.print("Text rotation", 0, 0);
myGLCD.setColor(0, 0, 255);
myGLCD.print("Arduino1", 0, 16, 0);
myGLCD.print("Arduino2", 220, 0, 90);
myGLCD.print("Arduino3", 220, 172, 180);
myGLCD.print("Arduino4", 0, 172, 270);

myGLCD.setFont(SmallFont);
myGLCD.setColor( 255 , 255 , 0 );
myGLCD.print("www.MiniArduino.com", 35 , 40 , 0);

myGLCD.setFont(BigFont);
myGLCD.setColor( 255 , 0 , 0 );
myGLCD.print("Counter", 55 , 130 , 0);

myGLCD.setFont(SevenSegNumFont);
myGLCD.setColor(0, 255, 0);
myGLCD.print(String(hours) , 45, 70 );
myGLCD.print(String(minutes) , 110 , 70);

while( true ) {
count++;

if( count%10000==0 ) {
  if (--minutes<0) {
      minutes=60;
      if(--hours<0) {
        hours=24;
      }
   }

displayDigits( hours , 45, 70);
displayDigits( minutes, 110 , 70);
//delay(1000);
     }
  }
}

