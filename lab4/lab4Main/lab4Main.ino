//main lab4 code

#include <Adafruit_CAP1188.h>

//pins
int fanPin = 2;
int lightPin = 4;
int firePlacePin = 6;
int buzzerPin = 8;

//on or off
bool fanOn = false;
bool lightOn = false;
bool fireOn = false;
bool buzzerOn = false;

Adafruit_CAP1188 cap = Adafruit_CAP1188();

void setup() {
  pinMode(fanPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(firePlacePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  cap.begin();
}

void loop() {

  uint8_t touched = cap.touched();

  if (touched == 0) {
    // No touch detected
    return;
  }
  
  delay(500);
  
  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {
      if ((i+1)==1){
        fanSwitch();
        Serial.println("fan");
      }
      else if ((i+1)==2) {
        lightSwitch();
        Serial.println("light");
      }
      else if((i+1)==3){
        fireSwitch();
        Serial.println("fire");
      }
      else if((i+1)==4){
        buzzerSwitch();
        Serial.println("buzzer");
      }
    }
  }
}

void buzzerSwitch(){
  if (buzzerOn){
          digitalWrite(buzzerPin, LOW);
          buzzerOn = false;
  }
  else {
          digitalWrite(buzzerPin, HIGH);
          buzzerOn = true;
  }
  return;
}

void fireSwitch(){
  if (fireOn){
          digitalWrite(firePlacePin, LOW);
          fireOn = false;
  }
  else {
          digitalWrite(firePlacePin, HIGH);
          fireOn = true;
  }
  return;
}

void lightSwitch(){
  if (lightOn){
          digitalWrite(lightPin, LOW);
          lightOn = false;
  }
  else {
          digitalWrite(lightPin, HIGH);
          lightOn = true;
  }
  return;
}

void fanSwitch(){
  if (fanOn){
          digitalWrite(fanPin, LOW);
          fanOn = false;
  }
  else {
          digitalWrite(fanPin, HIGH);
          fanOn = true;
  }
  return;
}
