//WeMos LOLION32 Lite alapú töltésmérő Pláth 2024.04.
#include <Wire.h>
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
const int freq = 500; const int res = 8; int faktor=1;
struct Button {
const uint8_t PIN;
uint32_t numberKeyPresses;
bool pressed;
};

Button button1 = {4, 0, false};//A GPIO 4. portján figyeljük az AutoZéró nyomógombot
void IRAM_ATTR ISR() {
	button1.numberKeyPresses++;
	button1.pressed = true;
}

int Auto_Zero = 13;//A DG444 IC 16. pin nullázó kapcsoló
int Meres_Hatar=15;//A DG444 IC 9. pin  méréshatárváltó kapcsoló
int GREEN1=32; int GREEN2=25;int RED1=26;int RED2=33;//A kjelzó LED-ek vezérlői
float Q=0; int m=0;
float Qmax=4.0;
String S="";  
float nanoC=4.5/2048;
int feszultseg = 0;
double zero=0;
void setup() {
  ledcAttach(GREEN1,freq, res);  ledcWrite(GREEN1,255);
  ledcAttach(GREEN2,freq, res);  ledcWrite(GREEN2,255);
  ledcAttach(RED1,freq, res);  ledcWrite(RED1,255);
  ledcAttach(RED2,freq, res);  ledcWrite(RED2,255);
  pinMode(Auto_Zero, OUTPUT);
  pinMode(Meres_Hatar, OUTPUT);
  digitalWrite(Meres_Hatar,HIGH);//Cf=520  pF HIGH OFF
  Serial.begin(9600);
  SerialBT.begin("Qmeter"); //Bluetooth eszközünk megjelenő neve
  delay(200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button1.PIN, INPUT_PULLUP);
	attachInterrupt(button1.PIN, ISR, FALLING);
  digitalWrite(LED_BUILTIN, LOW);//Jelzi, hogy elindult
  ledcWrite(GREEN1,0);ledcWrite(GREEN2, 0);ledcWrite(RED1, 0);ledcWrite(RED2, 0);//LEDEK bekapcsolása
  delay(500);
  ledcWrite(GREEN1, 255);ledcWrite(GREEN2, 255);ledcWrite(RED1, 255);ledcWrite(RED2, 255);//LEDEK kikapcsolása
  nullaz();
}

void loop() {
    if  (Serial.available() > 0) {
        S = Serial.readString() ;
        S.trim();
        decodS(); 
    }
    if  (SerialBT.available() > 0) {
      S = SerialBT.readString() ;
      S.trim();
      decodS();
    }
    if (button1.pressed) {
        nullaz();
        button1.pressed = false;
        digitalWrite(LED_BUILTIN, HIGH);//Start jelző ki
    }
  feszultseg = 0;
  for (int i=0;i<15;i++){
    feszultseg =  feszultseg + analogRead(27);
    delay(20);
  }
  feszultseg=feszultseg/15.0;
 
  m=feszultseg-zero; Q=-m*nanoC;
  //Serial.print(feszultseg); Serial.print("--- : "); 
  Serial.println(String(faktor*Q,2));SerialBT.println(String(faktor*Q,2));
  ledfeny();
  delay(20);
}


void nullaz(){
  ledcWrite(GREEN1,255);  ledcWrite(GREEN2,255);  ledcWrite(RED1,255);  ledcWrite(RED2,255);//Minden LED felvillan
  digitalWrite(Auto_Zero,LOW);  delay(500);  digitalWrite(Auto_Zero,HIGH);//DG444 ON delay(500) DG444 OFF
  feszultseg = 0;
  for (int i=0;i<15;i++){
    feszultseg =  feszultseg + analogRead(27);
    delay(20);
  }
  zero=feszultseg/15.0;//A zero-ban tárolom az alapállapothoz tartozó feszültséget
	delay(200);  
}

void decodS(){
  Serial.println(S);
  if (S=="-"){
    faktor=-1;
  }
  if (S=="+"){
    faktor=1;
  }
  if (S=="Z"){
        nullaz();
        S="";
      }
  if (S=="H"){
      nanoC=4.5/2048; //Méréshatár 4.5 nC H C=0,9 nF
      Qmax=4.0;
      digitalWrite(Meres_Hatar,HIGH);//DG444 OFF
      delay(100);
      nullaz();
      S="";
      }
      if (S=="L"){
        nanoC=50.0/2048;//Méréshatár 45 nC C=10 nF
        Qmax=45.0;
        digitalWrite(Meres_Hatar,LOW);//DG444 ON
        delay(100);
        nullaz();
      S="";  
  }
}

void ledfeny(){
 if(abs(Q)<0.1){Q=0;}
 ledcWrite(GREEN1, 255);ledcWrite(GREEN2, 255);ledcWrite(RED1, 255);ledcWrite(RED2, 255);//LEDEK kikapcsolása
  if (Q>0){
  //piros LED   
        if (abs(Q)<Qmax/2.0){
          ledcWrite(RED1, 255-255*abs(Q)/(Qmax/2.0));
        }else{
          ledcWrite(RED1, 0);//teljes fényerő
          Q=abs(Q)-Qmax/2;
          ledcWrite(RED2, 255-255*abs(Q)/(Qmax/2.0));
        }
  }else{
  //zöld LED
        if (abs(Q)<Qmax/2.0){
          ledcWrite(GREEN1, 255-255*abs(Q)/(Qmax/2.0));
        }else{
          ledcWrite(GREEN1, 0);//teljes fényerő
          Q=abs(Q)-Qmax/2;
          ledcWrite(GREEN2, 255-255*abs(Q)/(Qmax/2.0));
        }
  }
}
