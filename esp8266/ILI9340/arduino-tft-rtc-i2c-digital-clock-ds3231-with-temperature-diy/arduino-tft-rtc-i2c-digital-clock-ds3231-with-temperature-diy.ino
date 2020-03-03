// www.miniarduino.com
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#include <Wire.h>
#include "ds3231.h"

#define BUFF_MAX 128

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 1000;

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// These are the pins used for the arduino pro mini
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8

// Using software SPI is really not suggested, its incredibly slow
//Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
// Use hardware SPI
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

void setup() {
    Serial.begin(9600);
    Serial.println("GET time");
    tft.begin();
    tft.fillScreen(ILI9340_BLUE);
    tft.setRotation(3);
    Wire.begin();
    DS3231_init(DS3231_INTCN);
    memset(recv, 0, BUFF_MAX);
    //TIME SETUP comment back after uploading 
    //Serial.println("Setting time");
    //parse_cmd("T305121130032015",16);     // TssmmhhWDDMMYYYY
}

void loop(void) {

  char in;
    char tempF[6]; 
    float temperature;
    char buff[BUFF_MAX];
    unsigned long now = millis();
    struct ts t;

    // show time once in a while
    if ((now - prev > interval) && (Serial.available() <= 0)) {
        DS3231_get(&t); //Get time
        parse_cmd("C",1);
        temperature = DS3231_get_treg(); //Get temperature
        dtostrf(temperature, 5, 1, tempF);

        ////////////////////// DATE ////////////////////////
        tft.setTextColor(ILI9340_WHITE, ILI9340_BLUE);
        tft.setTextSize(2);
        tft.setCursor(15, 10);
        tft.print("Today is");
        tft.setCursor(115, 10);
        printMonth(t.mon); // month
        tft.setCursor(225, 10);
        tft.print(t.mday);
        
        tft.setCursor(260, 10);
        tft.print(t.year); // year
        
        tft.setTextSize(3);
        tft.setCursor(35, 50);
        tft.print("ARDUINO CLOCK");
        ///////////////////////// TIME //////////////////////
        tft.setCursor(35, 100);
        tft.setTextSize(5);
        tft.print(t.hour);
        tft.print(":");
        tft.setCursor(125, 100);
        if(t.min<10)
        {
          tft.print("0");
        }
        tft.print(t.min);
        tft.print(":");
        tft.setCursor(215, 100);
        if(t.sec<10)
        {
          tft.print("0");
        }
        tft.print(t.sec);
        //////////////////////// WEEK DAY ////////////////////
        tft.setTextSize(3);
        tft.setCursor(80, 150);
        printWeekday(t.wday); // day name
        //tft.print(t.wday);
        
        tft.setTextSize(1);
        tft.setCursor(100, 200);
        tft.print("www.miniarduino.com");
        /////////////////////// TEMPERATURE /////////////////
        tft.setCursor(30, 220); //Go to last line of the LCD Screen
        tft.setTextSize(2);
        tft.print("Temperature: ");
        tft.setCursor(180, 220); 
        tft.print(tempF);
        //tft.print((char)223);
        tft.print(" C");
        prev = now;
    }

    
    if (Serial.available() > 0) {
        in = Serial.read();

        if ((in == 10 || in == 13) && (recv_size > 0)) {
            parse_cmd(recv, recv_size);
            recv_size = 0;
            recv[0] = 0;
        } else if (in < 48 || in > 122) {;       // ignore ~[0-9A-Za-z]
        } else if (recv_size > BUFF_MAX - 2) {   // drop lines that are too long
            // drop
            recv_size = 0;
            recv[0] = 0;
        } else if (recv_size < BUFF_MAX - 2) {
            recv[recv_size] = in;
            recv[recv_size + 1] = 0;
            recv_size += 1;
        }

    }
}

void parse_cmd(char *cmd, int cmdsize)
{
    uint8_t i;
    uint8_t reg_val;
    char buff[BUFF_MAX];
    struct ts t;

    //snprintf(buff, BUFF_MAX, "cmd was '%s' %d\n", cmd, cmdsize);
    //Serial.print(buff);

    // TssmmhhWDDMMYYYY aka set time
    if (cmd[0] == 84 && cmdsize == 16) {
        //T355720619112011
        t.sec = inp2toi(cmd, 1);
        t.min = inp2toi(cmd, 3);
        t.hour = inp2toi(cmd, 5);
        t.wday = inp2toi(cmd, 7);
        t.mday = inp2toi(cmd, 8);
        t.mon = inp2toi(cmd, 10);
        t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
        DS3231_set(t);
        Serial.println("OK");
    } else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
        Serial.print("aging reg is ");
        Serial.println(DS3231_get_aging(), DEC);
    } else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
        DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
        //ASSMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // ss, mm, hh, dd
        }
        boolean flags[5] = { 0, 0, 0, 0, 0 };
        DS3231_set_a1(time[0], time[1], time[2], time[3], flags);
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
        DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
        //BMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // mm, hh, dd
        }
        boolean flags[5] = { 0, 0, 0, 0 };
        DS3231_set_a2(time[0], time[1], time[2], flags);
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 67 && cmdsize == 1) {  // "C" - get temperature register
        Serial.print("temperature reg is ");
        Serial.println(DS3231_get_treg(), DEC);
    } else if (cmd[0] == 68 && cmdsize == 1) {  // "D" - reset status register alarm flags
        reg_val = DS3231_get_sreg();
        reg_val &= B11111100;
        DS3231_set_sreg(reg_val);
    } else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
        reg_val = DS3231_get_addr(0x5);
        Serial.print("orig ");
        Serial.print(reg_val,DEC);
        Serial.print("month is ");
        Serial.println(bcdtodec(reg_val & 0x1F),DEC);
    } else if (cmd[0] == 71 && cmdsize == 1) {  // "G" - set aging status register
        DS3231_set_aging(0);
    } else if (cmd[0] == 83 && cmdsize == 1) {  // "S" - get status register
        Serial.print("status reg is ");
        Serial.println(DS3231_get_sreg(), DEC);
    } else {
        Serial.print("unknown command prefix ");
        Serial.println(cmd[0]);
        Serial.println(cmd[0], DEC);
    }
}

void printMonth(int month)
{
  switch(month)
  {
    case 1: tft.print(" January ");break;
    case 2: tft.print(" February ");break;
    case 3: tft.print(" March ");break;
    case 4: tft.print(" April ");break;
    case 5: tft.print(" May ");break;
    case 6: tft.print(" June ");break;
    case 7: tft.print(" July ");break;
    case 8: tft.print(" August ");break;
    case 9: tft.print(" September ");break;
    case 10: tft.print(" October ");break;
    case 11: tft.print(" November ");break;
    case 12: tft.print(" December ");break;
    default: tft.print(" Error ");break;
  } 
}
void printWeekday(int wday)
{
  switch(wday)
  {
    case 3: tft.print(" Monday ");break;
    case 4: tft.print(" Tuesday ");break;
    case 5: tft.print(" Wednesday ");break;
    case 6: tft.print(" Thursday ");break;
    case 7: tft.print(" Friday ");break;
    case 1: tft.print(" Saturday ");break;
    case 2: tft.print(" Sunday ");break;
  } 
}
