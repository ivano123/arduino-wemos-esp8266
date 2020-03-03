int pin=0;
float temp=0;
int timer = 100; 
int t1,t2;
void setup() { 
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 
  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); 
  Serial.begin(9600);   
} 

void d1(int p){
    switch(p){
      case 0:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 1); 
        digitalWrite(8, 0); 
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 1:{
        digitalWrite(2, 0); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 0); 
        digitalWrite(6, 0); 
        digitalWrite(7, 0); 
        digitalWrite(8, 0); 
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 2:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 0); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 0); 
        digitalWrite(8, 1); 
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }case 3:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 0); 
        digitalWrite(7, 0); 
        digitalWrite(8, 1);  
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 4:{
        digitalWrite(2, 0); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 0); 
        digitalWrite(6, 0); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1);  
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 5:{
        digitalWrite(2, 1); 
        digitalWrite(3, 0); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 0); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1);  
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 6:{
        digitalWrite(2, 1); 
        digitalWrite(3, 0); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1);  
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 7:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 0); 
        digitalWrite(6, 0); 
        digitalWrite(7, 0); 
        digitalWrite(8, 0);  
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 8:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1);  
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      case 9:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 0); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1);  
        digitalWrite(9,0);
        digitalWrite(10,1);
        break;
      }
      
    }
  }
  void d2(int p){
    switch(p){
      case 0:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 1); 
        digitalWrite(8, 0);  
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 1:{
        digitalWrite(2, 0); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 0); 
        digitalWrite(6, 0); 
        digitalWrite(7, 0); 
        digitalWrite(8, 0); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 2:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 0); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 0); 
        digitalWrite(8, 1); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 3:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 0); 
        digitalWrite(7, 0); 
        digitalWrite(8, 1); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 4:{
        digitalWrite(2, 0); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 0); 
        digitalWrite(6, 0); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 5:{
        digitalWrite(2, 1); 
        digitalWrite(3, 0); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 0); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 6:{
        digitalWrite(2, 1); 
        digitalWrite(3, 0); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 7:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 0); 
        digitalWrite(6, 0); 
        digitalWrite(7, 0); 
        digitalWrite(8, 0); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 8:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 1); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1); 
        digitalWrite(9,1);
        digitalWrite(10,0);
        break;
      }
      case 9:{
        digitalWrite(2, 1); 
        digitalWrite(3, 1); 
        digitalWrite(4, 1); 
        digitalWrite(5, 1); 
        digitalWrite(6, 0); 
        digitalWrite(7, 1); 
        digitalWrite(8, 1);
        digitalWrite(9,1);
        digitalWrite(10,0); 
        break;
      }
    }
  }
float suhu(){
  temp=analogRead(pin);
  temp=((temp/1024)*5000)/10;
  Serial.print("Suhu :");
  Serial.print(temp);
  Serial.println(" *C");
  return temp;
}
void pisah(float t){
  int x=(int)t;
  t1=x / 10;
  Serial.print(t1);
  t2=x % 10;
  Serial.print(t2);
}
void loop() { 
    pisah(suhu());
    for (int i=0; i<timer; i++){
      Serial.print(t1);
      d1(t1);
      delay(5);
      d2(t2);
      delay(5);
    }
}