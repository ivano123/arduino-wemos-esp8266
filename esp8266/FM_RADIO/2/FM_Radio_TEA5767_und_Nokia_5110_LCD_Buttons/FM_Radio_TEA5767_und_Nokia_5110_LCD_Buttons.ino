// TEA5767 und Nokia 5110 LCD Display

#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// D7 - Serial clock out (CLK oder SCLK)
// D6 - Serial data out (DIN)
// D5 - Data/Command select (DC oder D/C)
// D4 - LCD chip select (CE oder CS)
// D3 - LCD reset (RST)
Adafruit_PCD8544 lcd = Adafruit_PCD8544(7, 6, 5, 4, 3);

#include <Wire.h>

#define button_frequency_up     13
#define button_frequency_down   12
#define button_mute             11

#define TEA5767_mute_left_right  0x06
#define TEA5767_MUTE_FULL        0x80
#define TEA5767_ADC_LEVEL_MASK   0xF0
#define TEA5767_STEREO_MASK      0x80


int old_frequency=-1;
int frequency=10500;

byte old_stereo=0;
byte stereo=1;

byte old_mute=1;
byte mute=0;

byte old_signal_level=1;
byte signal_level=0;

unsigned long last_pressed;

void setup(void) {
  
  pinMode(button_frequency_up, INPUT);
  pinMode(button_frequency_down, INPUT);
  pinMode(button_mute, INPUT);
  
  Wire.begin();
  
  TEA5767_set_frequency();

  lcd.begin();
  lcd.setContrast(60);
  lcd.clearDisplay();
   
  set_text(1,2,"FM Radio",BLACK,1);  
  set_text(14,147,"blog.simtronyx.de",BLACK,1);
  
}


void loop() {

    if(frequency!=old_frequency){
      set_text(old_frequency>=10000?6:14,17,value_to_string(old_frequency),WHITE,2);
      set_text(frequency>=10000?6:14,17,value_to_string(frequency),BLACK,2);
      old_frequency=frequency;
    }
    
    TEA5767_read_data();
      
    if(old_stereo!=stereo){ 
        set_text(old_stereo?22:28,39,old_stereo?"Stereo":"Mono",WHITE,1);
        set_text(stereo?22:28,39,stereo?"Stereo":"Mono",BLACK,1);
        old_stereo=stereo;
    }
    
    if(old_signal_level!=signal_level){
        set_text(old_signal_level<10?76:70,39,String((int)old_signal_level),WHITE,1);
        set_text(signal_level<10?76:70,39,String((int)signal_level),BLACK,1);
        old_signal_level=signal_level;
        show_signal_level(signal_level);
    }
    
    if(old_mute!=mute){
        set_text(1,39,old_mute?"M":"S",WHITE,1);
        set_text(1,39,mute?"M":"S",BLACK,1);
        old_mute=mute;
    }
      
    delay(50);
    
  
  if(digitalRead(button_frequency_down)==HIGH){
    
    frequency=frequency-5;
    if(frequency<8750)frequency=10800;
    TEA5767_set_frequency();
  }
  if(digitalRead(button_frequency_up)==HIGH){
    
    frequency=frequency+5;
    if(frequency>10800)frequency=8750;
    TEA5767_set_frequency();
  }

  if(digitalRead(button_mute)==HIGH){
    
    TEA5767_mute();
  }
  
  delay(50);
  
}

unsigned char frequencyH = 0;
unsigned char frequencyL = 0;

unsigned int frequencyB;

unsigned char TEA5767_buffer[5]={0x00,0x00,0xB0,0x10,0x00};

void TEA5767_write_data(byte data_size){
   
  delay(50);
  
  Wire.beginTransmission(0x60);
  
  for(byte i=0;i<data_size;i++)
    Wire.write(TEA5767_buffer[i]);
  
  Wire.endTransmission();
  
  delay(50);
}

void TEA5767_mute(){ 
  
  if(!mute){   
    mute = 1;   
    TEA5767_buffer[0] |= TEA5767_MUTE_FULL;
    TEA5767_write_data(2);
//    TEA5767_buffer[0] &= ~TEA5767_mute;
//    TEA5767_buffer[2] |= TEA5767_mute_left_right;
  }   
  else{
    mute = 0;   
    TEA5767_buffer[0] &= ~TEA5767_MUTE_FULL;
    TEA5767_write_data(2);
//    TEA5767_buffer[0] |= TEA5767_mute;
//    TEA5767_buffer[2] &= ~TEA5767_mute_left_right;
  }
    
//  TEA5767_write_data(3);
}

void TEA5767_set_frequency()
{
  frequencyB = 4 * (frequency * 10000 + 225000) / 32768;
  TEA5767_buffer[0] = frequencyB >> 8;
  if(mute)TEA5767_buffer[0] |= TEA5767_MUTE_FULL;
  TEA5767_buffer[1] = frequencyB & 0XFF;
  
  TEA5767_write_data(5);
}

int TEA5767_read_data() {
  
  unsigned char buf[5];
  memset (buf, 0, 5);
  
  Wire.requestFrom (0x60, 5); 

  if (Wire.available ()) {
    for (int i = 0; i < 5; i++) {
      buf[i] = Wire.read ();
    }
        
    stereo = (buf[2] & TEA5767_STEREO_MASK)?1:0;
    signal_level = ((buf[3] & TEA5767_ADC_LEVEL_MASK) >> 4);
    
    return 1;
  } 
  else return 0;
}

void show_signal_level(int level){
  
  byte xs=68;
  byte ys=8;
  for(int i=0;i<15;i++){
    if(i%2!=0)lcd.drawLine(xs+i,ys,xs+i,ys-i/2,level>=i?BLACK:WHITE);
  }
}

void set_text(int x,int y,String text,int color,int textsize){
  
  lcd.setTextSize(textsize);
  lcd.setTextColor(color); 
  lcd.setCursor(x,y);     
  lcd.println(text);      
  lcd.display();         
}


String value_to_string(int value){
  
  String value_string = String(value / 100);
  value_string = value_string + '.' + ((value%100<10)?"0":"") + (value % 100);
  return value_string;
}
