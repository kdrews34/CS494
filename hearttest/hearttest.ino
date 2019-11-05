#include "MegunoLink.h"
#include "Filter.h"

// declare global variables
char mode;

// filter for breath
ExponentialFilter<float> Breath(10,0);
int bRaw = 0;

// set pinout and vars for the buzzer
int buzzer = 2;
unsigned char i,j;

// variables for calculating the BPM
int UpperThreshold = 700;
int LowerThreshold = 680;
int hReading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;

// variables for calculating inhalation/exhalation
int Max = 400;
int Min = 380;
float bReading = 0.0;
unsigned long ThirdPulseTime = 0;
unsigned long FourthPulseTime = 0;
unsigned long PulseIntervalin = 0;
unsigned long PulseIntervalout = 0;

void setup() {
  // initialize the serial communication:
  Serial.begin(19200);
  Serial.flush(); // clear receiver buffer
  pinMode(11, INPUT); // Setup for leads off detection LO +
  pinMode(10, INPUT); // Setup for leads off detection LO -
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // if there is incoming data from the serial read it
  if(Serial.available()){
    
    mode = Serial.read(); // read character
    Serial.flush(); // clear receiver buffer

    Serial.print(mode);
    
  } else {
    if(mode == 'f'){       
      fitnessMode();
    } else if (mode == 's'){
      stressMode();
    } else if (mode == 'm'){
      medMode();
    } else if (mode == 'l'){
      lungMode();
    }
  }
  delay(10);

}

// function which handles the fitness mode stuff
void fitnessMode(){

//  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
//    Serial.print(0);
//  } else {
//    hReading = analogRead(A0);
//    Serial.print(hReading);
//  }
  hReading = analogRead(A0);
    Serial.print(hReading);

  Serial.print(",");

  // filter the breath rate and send to the serial
  bRaw = analogRead(A4);
  Breath.Filter(bRaw);
  bReading = Breath.Current();
  Serial.print(bReading);

  getBPM(); //function call to calculate bpm
  getRR(); //function call to calculate rpm, inhalation, exhalation
  
}

// funcion which handles stress monitoring mode
void stressMode(){
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    Serial.print(0);
  } else {
    hReading = analogRead(A0);
    Serial.print(hReading);
  }

  Serial.print(",");

  // filter the breath rate and send to the serial
  bRaw = analogRead(A4);
  Breath.Filter(bRaw);
  bReading = Breath.Current();
  Serial.print(bReading);

  getBPM(); //function call to calculate bpm
  //getRR(); //function call to calculate rpm, inhalation, exhalation
}

void medMode(){
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    Serial.print(0);
  } else {
    hReading = analogRead(A0);
    Serial.print(hReading);
  }

  Serial.print(",");

  // filter the breath rate and send to the serial
  bRaw = analogRead(A4);
  Breath.Filter(bRaw);
  bReading = Breath.Current();
  Serial.print(bReading);

  getBPM(); //function call to calculate bpm
  //getRR(); //function call to calculate rpm, inhalation, exhalation
}

// function which handles the lung capacity mode
void lungMode(){
  // this is to send 0 value for the heart rate
  Serial.print(0);
  Serial.print(",");
  
  // filter the breath rate and send to the serial
  bRaw = analogRead(A4);
  Breath.Filter(bRaw);
  bReading = Breath.Current();
  Serial.print(bReading);
  Serial.print(",");
 
  // this 0 is to print 0 for the BPM
  Serial.println(0);
}

// function which makes activates the buzzer
void buzz(){
  //output an frequency
  for(i = 0; i < 80; i++) {
    digitalWrite(buzzer,HIGH);
    delay(2);//wait for 1ms
    digitalWrite(buzzer,LOW);
    delay(2);//wait for 1ms
  }
}

// function which gets bpm
void getBPM(){
  // Heart beat leading edge detected.
  if(hReading > UpperThreshold && IgnoreReading == false){
    if(FirstPulseDetected == false){
      FirstPulseTime = millis();
      FirstPulseDetected = true;
    }
    else{
      SecondPulseTime = millis();
      PulseInterval = SecondPulseTime - FirstPulseTime;
      FirstPulseTime = SecondPulseTime;
    }
    IgnoreReading = true;
  }

  // Heart beat trailing edge detected.
  if(hReading < LowerThreshold){
    IgnoreReading = false;
  }  

  BPM = (1.0/PulseInterval) * 60.0 * 1000;

  Serial.print(",");
  Serial.print((int)BPM);
}

// functoin which calculates and prints out the 
void getRR(){
  if(bReading > Max){
    FirstPulseTime = millis();
  }
  if(bReading < Min){
    SecondPulseTime = millis();
    PulseIntervalout = SecondPulseTime - FirstPulseTime;
  }

  // print out the Pulse Intervalout in millis
  Serial.print(",");
  Serial.print(PulseIntervalout);
       
  if(bReading < Min){
    ThirdPulseTime = millis();
  }
  if(bReading > Max){
    FourthPulseTime = millis();
    PulseIntervalin = FourthPulseTime - ThirdPulseTime;
  }

  // print out the Pulse IntervalIn in millis
  Serial.print(",");
  Serial.println(PulseIntervalin);

}
