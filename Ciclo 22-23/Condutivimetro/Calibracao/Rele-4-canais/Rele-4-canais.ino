#define PINO_R1 1
#define PINO_R2 2
#define PINO_R3 3
#define PINO_R4 4

#define PINO_A0 A0

void setup() {
  pinMode(PINO_R1, OUTPUT);
  pinMode(PINO_R2, OUTPUT);
  pinMode(PINO_R3, OUTPUT);
  pinMode(PINO_R4, OUTPUT);
  pinMode(PINO_A0, INPUT);
  Serial.begin(115200);
}

void loop() {

  ligarRele(PINO_R1);
  Serial.print(analogRead(0));
  Serial.print(",");
  ligarRele(PINO_R2);
  Serial.print(analogRead(0));
  Serial.print(",");
  ligarRele(PINO_R3);
  Serial.print(analogRead(0));
  Serial.print(",");
  ligarRele(PINO_R4);
  Serial.print(analogRead(0));
  Serial.println();
  delay(500);
  


}

void ligarRele(int porta) {
  if (porta == PINO_R1) {
    digitalWrite(PINO_R1, HIGH);
    digitalWrite(PINO_R2, LOW);
    digitalWrite(PINO_R3, LOW);
    digitalWrite(PINO_R4, LOW);
  } else if (porta == PINO_R2) {
    digitalWrite(PINO_R1, LOW);
    digitalWrite(PINO_R2, HIGH);
    digitalWrite(PINO_R3, LOW);
    digitalWrite(PINO_R4, LOW);
  }
  else if (porta == PINO_R3) {
    digitalWrite(PINO_R1, LOW);
    digitalWrite(PINO_R2, LOW);
    digitalWrite(PINO_R3, HIGH);
    digitalWrite(PINO_R4, LOW);
  } else if (porta == PINO_R4) {
    digitalWrite(PINO_R1, LOW);
    digitalWrite(PINO_R2, LOW);
    digitalWrite(PINO_R3, LOW);
    digitalWrite(PINO_R4, HIGH);
  }
  delay(500);
}
