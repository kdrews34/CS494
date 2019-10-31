//Test digital outputs

//pins
int fanPin = 2;
int lightPin = 4;
int firePlacePin = 6;
int buzzerPin = 8;

void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(firePlacePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(fanPin, HIGH);
  delay(1000);
  digitalWrite(fanPin, LOW);

  digitalWrite(lightPin, HIGH);
  delay(1000);
  digitalWrite(lightPin, LOW);

  digitalWrite(firePlacePin, HIGH);
  delay(1000);
  digitalWrite(firePlacePin, LOW);

  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}