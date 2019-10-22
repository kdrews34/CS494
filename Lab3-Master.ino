#include <MPU6050.h>

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 13
bool blinkState = false;

// var that holds the mode
String mode;

int UpperThreshold = 200;
int LowerThreshold = 250;
int stepThresh = 200;
int FSRMF =0;
int FSRLF =0;
int FSRMM =0;
int FSRH = 0;
float Cadence = 0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
float Time = 0;
float ts = 0;
float stride = 0.0;
float stepl = 0.0;
float spd = 0.0;
int stepcount = 0;

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  
  // put your setup code here, to run once:
  Serial.begin(19200);
  
  accelgyro.initialize(); // initialize device
  
  Serial.flush(); // clear receiver buffer
  pinMode(3, OUTPUT); // initializing LEDs
  pinMode(5, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(11, OUTPUT); 
}

void loop() {
  // if their is incoming data from the serial read it
  if(Serial.available()){

    mode = Serial.readString(); // read string
    Serial.flush(); // clear the receiver buffer
  } else {
    if(mode.equals("secI")){
      sectionI();
    } else if (mode.equals("secII")){
      sectionII();
    } else if (mode.equals("secIII")){
      sectionIII();
    } else if (mode.equals("secIV")){
      sectionIV();
    }
  }
}


void sectionI(){
  // put your main code here, to run repeatedly:
  FSRMF = analogRead(A0);
  FSRLF = analogRead(A1);
  FSRMM = analogRead(A2);
  FSRH = analogRead(A3);
  
  
  //CODE FOR STEP TIME AND RELATED VALUES
  if(FSRH < UpperThreshold && IgnoreReading == false){
    if(FirstPulseDetected == false){
      FirstPulseTime = millis();
      FirstPulseDetected = true;
  
    }
    else{
      stepcount = stepcount+1;
      SecondPulseTime = millis();
      Time = SecondPulseTime - FirstPulseTime;
      FirstPulseTime = SecondPulseTime;
    }
    IgnoreReading = true;
  }
  
  // Heart beat trailing edge detected.
  if(FSRH > LowerThreshold){
    IgnoreReading = false;
  } 
  
  if(FSRMF<stepThresh){
    digitalWrite(3,HIGH);
  }else{
    digitalWrite(3,LOW);
  }
  
  if(FSRLF<stepThresh){
    digitalWrite(5,HIGH);
  }else{
    digitalWrite(5,LOW);
  }
  
  if(FSRMM<stepThresh){
    digitalWrite(9,HIGH);
  }else{
    digitalWrite(9,LOW);
  }
  
  if(FSRH<stepThresh){
    digitalWrite(11,HIGH);
  }else{
    digitalWrite(11,LOW);
  }
  
  //ts = Time/1000.0; // commented because it prevents speed and cadence from displaying 
  stride = (.65*Time);
  stepl = stride/2.0;
  Cadence = (1.0/Time) * 60.0 * 1000.0;
  spd = stride/Time;

  Serial.print(FSRMF);
  Serial.print(",");
  Serial.print(FSRLF);
  Serial.print(",");
  Serial.print(FSRMM);
  Serial.print(",");
  Serial.print(FSRH);
  Serial.print(",");
  Serial.print(stepl);
  Serial.print(",");
  Serial.print(stride);
  Serial.print(",");
  Serial.print(Cadence);
  Serial.print(",");
  Serial.print(spd);
  Serial.print(",");
  Serial.println(stepcount);

  delay(10);
}

void sectionII(){
  FSRMF =  analogRead(A1);
  FSRLF =  analogRead(A0);
  FSRMM =  analogRead(A2);
  FSRH =  analogRead(A3);

  // print out the data from the force sensors
  Serial.print(FSRMF);
  Serial.print(",");
  Serial.print(FSRLF);
  Serial.print(",");
  Serial.print(FSRMM);
  Serial.print(",");
  Serial.print(FSRH);


  // calculate and print out profile
  if ( ( FSRLF + FSRMF ) < (FSRH) ) {
    Serial.println(",Heel");
  }
  else if ( ( FSRMF ) > (FSRH)) {
    Serial.println(",Tiptoe");
  }
  else if ((FSRLF + FSRH) < (FSRMM + FSRMF)) {
    Serial.println(",Intoe");
  }
  else if ( (FSRLF + FSRH) > (FSRMM + FSRMF)) {
    Serial.println(",Outoe");
  }
  else {
    Serial.println(",None");
  }

  delay(10);
}

void sectionIII(){
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if ( !(ay > 10000) && !(ay < 7000) && !( az > 3000) && !( az < -3000)) {
    Serial.println("Not in Motion") ;
  }
  else  if ( az < -3000 ) {
    Serial.println( "Moving Forward" );
  }
  else if ( az > 3000) {
    Serial.println ("Moving Backwards");
  }
  else  if ( ay < 7000 ) {
    Serial.println ("Moving Left");
  }
  else if ( ay > 10000) {
    Serial.println ("Moving Right");
  }

  delay(100);
}

void sectionIV(){
  FSRMF =  analogRead(A1);

  Serial.println(FSRMF);

  delay(10);
}
