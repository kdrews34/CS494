#include "MegunoLink.h"
#include "FIlter.h"

ExponentialFilter<float> Breath(10,0);
ExponentialFilter<float> Heart(10,0);
float bRaw;
float hRaw;

void setup() {
  // initialize the serial communication:
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  Serial.begin(115200);
}

void loop() {

  //print breathing rate
  bRaw = analogRead(A1);
  Breath.Filter(bRaw);

  //print heart rate
  if(!((digitalRead(10) == 1)||(digitalRead(11) == 1))){
      hRaw = analogRead(A0);
      Heart.Filter(hRaw);
  }
  //print stuff to serial
  Serial.print(Breath.Current());
  Serial.print(",");
  Serial.println(Heart.Current());
  //Wait for a bit to keep serial data from saturating
  delay(10);
}
