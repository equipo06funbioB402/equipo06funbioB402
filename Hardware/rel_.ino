int RELE=2;

void setup() {
  pinMode(RELE, OUTPUT);

}

void loop() {
  digitalWrite(RELE, LOW);
  delay(5000);
  digitalWrite(RELE, HIGH);
  delay(1200000);
}
