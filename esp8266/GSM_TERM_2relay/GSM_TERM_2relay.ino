#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN  4
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
SMSGSM sms;

int Rel1 = 11; // для реле 1
int Rel2 = 12; // для реле 2
 

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];
String input_string = "";
String smsContent = "";
char pos;
int flag=0;

char n1[] = "+79170417032";
char sendsms[160];

float hum, tem;
DHT dht(DHTPIN, DHTTYPE);

void setup() {

 dht.begin(); 
 
// gprsSerial.begin(4800);
 pinMode(Rel1, OUTPUT);
 pinMode(Rel2, OUTPUT);
 digitalWrite(Rel1, HIGH);
 digitalWrite(Rel2, HIGH);

Serial.begin(115200);

       if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");

}
void loop() {
delay(2000);
  gettemperature();
Serial.println("temp: "+String(tem)+" hum: "+String(hum));
smsContent=("Temperature: "+String(tem)+" Vlajnost: "+String(hum));
smsContent.toCharArray(sendsms,160);

if (tem<=12) {digitalWrite(Rel1, LOW);flag=1;}
if (tem<=10) {digitalWrite(Rel2, LOW);sms.SendSMS(n1, "t < +10C, Vklucheno RELE 2, Kriticheskaya temperatura");flag=1;}
if (tem>=17&&flag==1) {digitalWrite(Rel1, HIGH);digitalWrite(Rel2, HIGH);flag=0;}

pos = sms.IsSMSPresent(SMS_UNREAD); // смотрим непрочитанные СМС
      if (pos) {                    // Если непрочитанные СМС есть, то...
        
getsms(); // получаем непрочитанную СМС                    // Если непрочитанные СМС есть, то...
        


if (input_string=="11"){digitalWrite(Rel1, LOW);sms.SendSMS(n1, "Prinuditelnoe Vkluchenie RELE 1");memset(n,0,20);}
else if (input_string=="21"){digitalWrite(Rel2, LOW);sms.SendSMS(n1, "Prinuditelnoe Vkluchenie RELE 2");memset(n,0,20);flag=1;}
else if (input_string=="10"){digitalWrite(Rel1, HIGH);sms.SendSMS(n1, "Prinuditelnoe otkluchenie RELE 1");memset(n,0,20);}
else if (input_string=="20"){digitalWrite(Rel2, HIGH);sms.SendSMS(n1, "Prinuditelnoe otkluchenie RELE 2");memset(n,0,20);flag=0;}
else if (input_string=="0"){sms.SendSMS(n1, sendsms);memset(n,0,20);}

sms.DeleteSMS(pos);
delay(1000);


}
}

void getsms()
{
          sms.GetSMS(pos,n,20,smsbuffer,160);
//          Serial.println(n);
//          Serial.println(smsbuffer);
          input_string =String(smsbuffer);
          Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:--------- " + input_string + "\n");
}

void gettemperature() {
    hum = dht.readHumidity();
    tem = dht.readTemperature(false);
}
