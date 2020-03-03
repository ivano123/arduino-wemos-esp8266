#include <TinyGPS++.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
 

#define TELLNUMBER "70001112233"                       // номен телефона для команды
#define SMSNUMBER "AT+CMGS=\"+70001112233\""           // номер на который будем отправлять SMS

#define LED 13

boolean GSMSMSflag = 0;           // команда на отправку SMS

TinyGPSPlus gps; 

SoftwareSerial gsm(15,13);         // программный UART для GSM модема
SoftwareSerial ss(3, 4);         // программный UART для GPS приемника
#define ss Serial1                // аппаратный UART в леонардо для GPS приемника
 
 

void setup()
{
  delay(1000);
  Serial.begin(9600);         
  ss.begin(9600);                // скорость GPS
  gsm.begin(9600);               // скорость GSM M590
  
  delay(100);
  Serial.println("start");
  pinMode(LED, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
pinMode(A0, INPUT_PULLUP); 

  
  GSMSMSflag = EEPROM.read(0);    //  достаем флаг 
  
Serial.println("modem"); 
            // нужно дождатся включения модема и соединения с сетью
  delay(2000);    
//  while(gsm.find("STARTUP"));          /// ждем команды от модема  
  gsm.println("ATE0");                   // выключаем эхо  
  
  while(1){                              // ждем подключение модема к сети
        gsm.println("AT+COPS?");
        if (gsm.find("+COPS: 0")) break;
           digitalWrite(LED, LOW);               // блымаем светодиодом
           delay(50);  
           digitalWrite(LED, HIGH);  
           delay(500); 
   }
    
     gsm.println("AT+CMGF=1");            //настройки для SMS
     delay(100);
     gsm.println("AT+CSCS=\"GSM\"");   
           
Serial.println("Modem OK"); 
     digitalWrite(LED, LOW);               // блымаем светодиодом
     delay(1500);  
     digitalWrite(LED, HIGH);
     delay(250); 
     digitalWrite(LED, LOW);  

}


void loop(){

  if (GSMSMSflag == 0){                     // если флаг ожидания команды от модема
  // ищим RING   
    if(gsm.find("RING")){                    // если нашли RING
      gsm.println("AT+CLIP=1");              // включаем АОН, 
            
      while(1){                              // в цикле
       if (gsm.find(TELLNUMBER)){            // ищим номер телефона, если нашли
         GSMSMSflag = 1;                     // меняем режим  
         EEPROM.write(0, GSMSMSflag);        // пишим его в еепром
         break;                              // и выходим
       }  
       else{                                 // иначе 
         gsm.println("AT+CPAS");             // спрашиваем состояние модема   
         delay(100);
         if (gsm.find("+CPAS: 0")) break;    // и если он в "готовности", выходим из цикла
       }                                     // если звонок в процессе, возвращает +CPAS: 3
      }                                      // и крутимся дальше
       
        gsm.println("AT+CLIP=0");            // выключаем АОН, 
        delay(500);
        gsm.println("ATH0");                 // сбрасываем вызов 

         digitalWrite(LED, LOW);             // сигнализируем об этом
         delay(500);  
         digitalWrite(LED, HIGH);
         delay(250); 
         digitalWrite(LED, LOW);   
   
    } 
  }
    //////////// GPS
  
  if (GSMSMSflag == 1){                               //  если сменился режим
    do{          
      do{                                             //  в цикле забераем данные от GPS
         while (ss.available() > 0) gps.encode(ss.read());
            displayInfo();    // !!! это тут только для проверки                      
      }while (!gps.location.isUpdated() && gps.location.age() > 1500);   
    }while (!gps.location.isValid());                  //  циклЫ крутим пока не убедимся что получили свежие данные  

Serial.println("SMS!");  
      GSMsendSMS();                           // отправляем SMS
      GSMSMSflag = 0;                         // меняем режим  
      EEPROM.write(0, GSMSMSflag);            // пишим его в еепром
      
  }
//  
  

  
//if(!digitalRead(A0)) GSMSMSflag = 1;         // если нажали кнопку, для отладки
   
}
//////
      ///                                   // отправляет SMS
void GSMsendSMS(){ 
      delay(100);
      
      gsm.println(SMSNUMBER);               // отправляем команду на отправку смс
      delay(100);
      gsm.print(gps.location.lat(), 6);     // передаем координаты
      gsm.print(",");
      gsm.print(gps.location.lng(), 6); 

      gsm.print(" S:");                     // передаем скорость движения
      gsm.print(gps.speed.kmph());

      gsm.print(" T:");                     // передаем время с GPS
      if (gps.time.hour() < 10) gsm.print("0");
      gsm.print(gps.time.hour());
      gsm.print(":");
      if (gps.time.minute() < 10) gsm.print(F("0"));
      gsm.print(gps.time.minute());
      
      gsm.print((char)26);                  // символ завершающий передачу
Serial.println("ok");
     
}


 
/// отправляет в сериал инфу с GPS приемника, оставлено для проверки и отладки
void displayInfo(){
  
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  } 
 
  Serial.print(F(" "));
  if (gps.speed.isValid())  {
    Serial.print(gps.speed.kmph());
  }
  
  Serial.println();
}

