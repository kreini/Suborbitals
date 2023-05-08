/*****************************************************************/
/* Avionics communication emulator code for testing OBC emulator */
/*****************************************************************/
/* Kreinicker Gabor */
/* BME Suborbitals  */
/* 2023.04.25.      */
/********************/

String msg;
int cnt;

void setup() {
  Serial.begin(9600);
  cnt = 1;
}

void loop() {
  
  int checksum = calculateChecksum(message());

  Serial.println(message() + String(checksum, HEX));

  // Wait for a response
  // while (Serial.available() == 0);
  // String response = Serial.readStringUntil('\n');
  // Serial.println(response);
  if (cnt < 99)
    cnt++;
  else
    cnt = 1;
  
  delay(1000);
}


String message() {
  if (cnt < 10)
    msg = ("$TRMIT,PARAMETER1,PARAMETER2#0" + String(cnt) + "*");
  else
    msg = ("$TRMIT,PARAMETER1,PARAMETER2#" + String(cnt) + "*");
  return msg;
}


int calculateChecksum(String message) {
  int checksum = 0;
  for (int i = 0; i < message.length(); i++) {
    if (message.charAt(i) != '$' && message.charAt(i) != '*') {
      checksum ^= message.charAt(i);
    }
  }
  return checksum;
}
