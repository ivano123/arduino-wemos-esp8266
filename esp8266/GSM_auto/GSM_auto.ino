#include "SIM900.h"
#include <SoftwareSerial.h>

#include "sms.h"
#include "call.h"
SMSGSM sms;
CallGSM call;

//=================Пины 2 и 3 для Подключения GSM модуля


int flag=0;
boolean started=false;
char smsbuffer[160];
char n[20];
String n1 = "+79371662772";
String input_string = "";
String smsContent = "";

char smsDv[] ="Komanda Vipolnena!";
char pos;
char sendsms[160];

void setup() {

 pinMode(7, OUTPUT);
digitalWrite(7,HIGH);
Serial.begin(9600);


       if (gsm.begin(4800)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");

}
void loop() {

pos = sms.IsSMSPresent(SMS_UNREAD); // смотрим непрочитанные СМС
      if (pos) {                    // Если непрочитанные СМС есть, то...
        
getsms(); // получаем непрочитанную СМС

if (input_string=="1"){Serial.print("YES!");
//n1.toCharArray(n,20);
sms.SendSMS(n, smsDv);
sms.DeleteSMS(1);
memset(n,0,20); // Команда "1" - действие
input_string=""; 
char smsbuffer[160]="";
digitalWrite(7,LOW);
delay(2000);
digitalWrite(7,HIGH);
}
sms.DeleteSMS(pos); // удаляем СМС  из симки
      }
}
void getsms()
{
sms.GetSMS(pos,n,20,smsbuffer,160);
input_string =String(smsbuffer);
          }
