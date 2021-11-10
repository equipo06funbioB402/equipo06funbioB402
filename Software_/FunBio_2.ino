#include <SoftwareSerial.h> //comunicación serial del arduino con el módulo dfplayer
#include <LiquidCrystal_I2C.h> //librería del LCD
#include <DFRobotDFPlayerMini.h>
SoftwareSerial mySoftwareSerial(12, 10); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
                            //C F
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int RELE = 9;
const int tonos[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};
const int countTonos = 10;
int poten = A3; //potenciometro

int ocupado = 6;

int reproducir = 5;
int pausa = 4;
int siguiente = 3;
int anterior = 2;

int ant, pro, par, rep, pot, VOL, VOL_OLD, ocu;
int estado = 1;
int flag = 0;

byte play_[] = {B01000, B01100, B01110, B01111, B01110, B01100, B01000, B00000};
byte pause_[] = {B00000, B01010, B01010, B01010, B01010, B01010, B00000, B00000};

int play_0 = play_;
int pause1 = pause_;
int contseg = 0;
int contmin = 0;
int playing = 0;

void sonido() {
  for (int iTono = 0; iTono < countTonos; iTono++)
  {
    tone(RELE, tonos[iTono]);
    delay(50);
  }
  noTone(RELE);
}


void setup() {
  pinMode(RELE, OUTPUT);
  pinMode(poten, INPUT);
  pinMode(reproducir, INPUT);
  pinMode(pausa, INPUT);
  pinMode(siguiente, INPUT);
  pinMode(anterior, INPUT);
  pinMode(ocupado, INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.createChar(0, play_0);
  lcd.createChar(1, pause1);
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.print("Reproducir música");
  lcd.write(0);
  lcd.leftToRight();
  lcd.setCursor (0, 1);
  lcd.print("** para iniciar **");
  delay(3000);
  sonido();
  VOL_OLD = 0; //para poder identificar cambios de volumen
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(15); //0 - 30
  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.disableLoopAll();
  lcd.setCursor (7, 1);
  lcd.print("Presionar");
}


void loop()
{
  ant = digitalRead(anterior);
  pro = digitalRead(siguiente);
  par = digitalRead(pausa);
  rep = digitalRead(reproducir);
  pot = analogRead(poten);
  ocu = digitalRead(ocupado);
  if ((ocu == 1) && (flag == 0)) { // 0 tocando. 1 Detenido
    lcd.setCursor(0, 0);
    lcd.write(0);
    lcd.print("Robotecs Music");
    lcd.write(0);
    lcd.leftToRight();
    lcd.setCursor (7, 1);
    lcd.print("PressPlay");
    delay(20);
  }
  // conteo
  if (ocu == 0) { // playing==1
    contseg++;
    Serial.println("segundos");
    Serial.println(contseg);
    Serial.println("playing");
    Serial.println(playing);
    lcd.setCursor(11, 1);
    lcd.print('0');
    lcd.print(contmin);
    lcd.print(":");
    lcd.print('0');
    lcd.print('0');
    if (contseg < 10) {
      lcd.setCursor(15, 1);
    }
    else if (contseg >= 10) {
      lcd.setCursor(14, 1);
      if (contseg > 59) {
        contmin++;
        lcd.setCursor(11, 1);
        lcd.print('0');
        lcd.print(contmin);
        lcd.print(":");
        contseg = 0;
      }
    }
    lcd.print(contseg);
    delay(1000);
  }
  //conteo
  VOL = map(pot, 0, 1023, 0, 30);
  delay(20);
  if (VOL != VOL_OLD) {
    VOL_OLD = VOL;
    lcd.setCursor(0, 1);
    lcd.print("Vol:");
    if (VOL >= 10) {
      lcd.setCursor(4, 1);
    }
    else if (VOL < 10) {
      lcd.print('0');
    }
    lcd.print(VOL);
    lcd.print(" ");
    myDFPlayer.volume(VOL);
  }

  if (rep == LOW) {
    delay(2);
    myDFPlayer.play();
    reprod();
  }
  if (anterior == LOW) {
    delay(2);
    myDFPlayer.previous();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Anterior...");
    contseg = 0;
    contmin = 0;
    delay(1000);
    reprod();
  }
  if (siguiente == LOW) {
    myDFPlayer.next();
    delay(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Siguiente...");
    delay(1000);
    contseg = 0;
    contmin = 0;
    reprod();
  }
  if (pausa == LOW) {
    if (flag == 0) {
      delay(20);
      flag = 1;
      playing = 0;
      myDFPlayer.pause();
      lcd.setCursor(0, 0);
      lcd.print("Pause  ");
      lcd.setCursor(12, 0);
      lcd.write(2); //0 - 1 - 2
      delay(1000);
    }
    else if (flag == 1) {
      delay(20);
      myDFPlayer.start();
      flag = 0;
      reprod();
    }
  }
}
void reprod() {
  lcd.clear();
  lcd.setCursor(0, 0);
  playing = 1;
  lcd.print("Playing");
  delay(1000);
  lcd.setCursor(9, 0);
  lcd.print('0');
  lcd.print(myDFPlayer.readCurrentFileNumber()); //actual
  lcd.print(" ");
  lcd.write(1);
  flag=0;
  delay(1000);
}
