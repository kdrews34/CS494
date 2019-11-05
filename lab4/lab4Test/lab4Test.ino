//Test digital outputs

//pins
int fanPin = 8;
int lightPin = 4;
int firePlacePin = 6;
int buzzerPin = 2;

void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(firePlacePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  delay(5000);
  digitalWrite(fanPin, HIGH);
  delay(1000);
  digitalWrite(fanPin, LOW);
  delay(1000);
  digitalWrite(lightPin, HIGH);
  delay(1000);
  digitalWrite(lightPin, LOW);
  delay(1000);
  digitalWrite(firePlacePin, HIGH);
  delay(1000);
  digitalWrite(firePlacePin, LOW);
  delay(1000);
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  delay(1000);
  digitalWrite(lightPin, HIGH);
  delay(1000);
  digitalWrite(firePlacePin, HIGH);
  delay(1000);
  digitalWrite(firePlacePin, LOW);
  delay(1000);
  digitalWrite(lightPin, LOW);
  delay(10000);
}
