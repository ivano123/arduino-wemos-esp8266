/*   arduino       SIM800L
       D2           RING
       D4            TX
       D5            RX
       D6           DTR     */

#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"
#include <avr/sleep.h>
#include <avr/power.h>
 
int pin = 2; // подключается к ring пину gsm модуля
int relay = 9; // реле
byte stat=0;

SMSGSM sms;
CallGSM call;

void setup(){
set_sleep_mode(SLEEP_MODE_STANDBY); //Определяем режим сна
pinMode(pin, INPUT);
digitalWrite(pin,HIGH);

     pinMode(6, OUTPUT); // к DTR пину GSM модуля
     digitalWrite(6,LOW); // пробуждаем GSM модуль
     
     pinMode(relay, OUTPUT);
     digitalWrite(relay,HIGH);

     Serial.begin(115200);
     Serial.println("GSM test.");
      
     if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          
     } else Serial.println("\nstatus=IDLE");
     
     delay(300);

     gsm.SimpleWrite(F("AT+CSCLK=1"));
     gsm.SimpleWriteln("");

}

void loop(){

stat=call.CallStatus();
      
    if(stat==CALL_INCOM_VOICE) {
 //     Serial.println("CALL!!!");
    //  sms.SendSMS("+79371662772", "Vipolneno!");
      digitalWrite(relay,LOW);
      delay(2000);
      digitalWrite(relay,HIGH);
      call.HangUp(); // Кладем трубку
    }

  digitalWrite(6,HIGH); // вырубаем GSM модуль
  EnterSleep(); //Пора спать
    
}

void wakeUp()
{
  Serial.println("WakeUp"); //Проснулись
  detachInterrupt(0); //Отключаем прерывания 
  digitalWrite(6,LOW); // пробуждаем GSM модуль
  delay(500);
}

void EnterSleep()
{
  attachInterrupt(0, wakeUp, LOW); //Если на 0-вом прерываниии - ноль, то просыпаемся.
  delay(100);
 
  sleep_enable(); //Разрешаем спящий режим
  sleep_mode(); //Спим (Прерывания продолжают работать.) Программа останавливается.
  sleep_disable(); //Запрещаем спящий режим
}
