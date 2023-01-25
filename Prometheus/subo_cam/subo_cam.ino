void setup() {
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(3000);
  digitalWrite(12, HIGH);
  delay(1000);
}

void loop() {
  digitalWrite(12, LOW);
  delay(200);
  digitalWrite(12, HIGH);
  delay(5000);
  digitalWrite(12, LOW);
  delay(200);
  digitalWrite(12, HIGH);
  delay(200);
}
