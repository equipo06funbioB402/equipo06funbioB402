#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Pulsadores
const int s1 = 2; // pulsadores

const int RELE=4;

int state1 = 0; // estados para las entradas digitales
int last1 = 1;


SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


void setup() {
 mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(10);  //el volumen va de 1-30
  myDFPlayer.volumeUp(); //subir volumen
  myDFPlayer.volumeDown(); //bajar voolumen

  //ecualizacion
  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);

  //almacenamietno de la música
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  pinMode(s1, INPUT_PULLUP);

  pinMode(RELE, OUTPUT);
  

}

void loop() {
state1 = digitalRead(s1);
 digitalWrite(RELE, LOW);
  delay(5000);
  
  if (state1 != last1) {
    if (state1 == HIGH) {
      myDFPlayer.play(1);
      Serial.print(state1);
      delay(1000);
    }
  }
  last1  = state1;
  digitalWrite(RELE, HIGH);
  delay(1200000);
  
}
