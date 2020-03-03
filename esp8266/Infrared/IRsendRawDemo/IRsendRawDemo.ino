/*
 * IRremote: IRsendRawDemo - demonstrates sending IR codes with sendRaw
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * IRsendRawDemo - added by AnalysIR (via www.AnalysIR.com), 24 August 2015
 *
 * This example shows how to send a RAW signal using the IRremote library.
 * The example signal is actually a 32 bit NEC signal.
 * Remote Control button: LGTV Power On/Off. 
 * Hex Value: 0x20DF10EF, 32 bits
 * 
 * It is more efficient to use the sendNEC function to send NEC signals. 
 * Use of sendRaw here, serves only as an example of using the function.
 * 
 */


#include <IRremote.h>

IRsend irsend;

void setup()
{

}

void loop() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  //unsigned int irSignal[] = {9000, 4500, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 39416, 9000, 2210, 560}; //AnalysIR Batch Export (IRremote) - RAW
  // вкл - выкл телевизора самсунг
//  unsigned int  irSignal[] = {4450,4550, 550,1700, 500,1700, 550,1700, 550,600, 500,600, 500,600, 550,600, 500,600, 550,1700, 500,1750, 500,1700, 550,600, 500,600, 550,550, 550,600, 500,600, 550,600, 500,1700, 550,600, 500,600, 500,600, 550,600, 500,600, 550,600, 500,1700, 550,600, 500,1700, 550,1700, 550,1700, 500,1700, 550,1700, 550,1700, 500};  // SAMSUNG E0E040BF
// кондишн
 unsigned int  irSignal[] = {3300,1650, 400,1300, 400,450, 350,450, 400,450, 400,400, 400,450, 400,450, 400,450, 350,450, 400,450, 400,400, 400,450, 400,1250, 400,450, 400,450, 400,400, 400,450, 400,450, 400,400, 400,450, 400,450, 400,450, 350,450, 400,450, 400,400, 400,450, 400,450, 400,400, 400,450, 400,450, 400,1250, 400,450, 400,1250, 400,1250, 400,1300, 400,1250, 400,1250, 400,1300, 400,400, 400,1300, 350,1300, 400,1250, 400,1300, 350,1300, 400,1250, 400,1300, 350,1300, 400,1250, 400,450 };  
 
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.

  delay(5000); //In this example, the signal will be repeated every 5 seconds, approximately.
}
