#include "MegunoLink.h"
#include "Filter.h"
#include "PulseSensorPlayground.h"
#include "Time.h"
#include "Timelib.h"

// declare global variables
String cmd;
int age;
String mode;

ExponentialFilter<float> Breath(10,0);
ExponentialFilter<float> Heart(10,0);
int bRaw;
int hRaw;
int BPM = 0;
float r1=0;
float r2=0;
int flag=0;
int flag_r=0;
float ecg;
int hh;

bool aboveThreshold = false;
const int heartPin = 0;
int threshold = 500;
int counter = 0;


void setup() {
  // initialize the serial communication:
  Serial.begin(19200);
  Serial.flush(); // clear receive buffer
  pinMode(11, INPUT); // Setup for leads off detection LO +
  pinMode(10, INPUT); // Setup for leads off detection LO -
}

void loop() {
  // if there is incoming from the serial read it
  if(Serial.available()){
    cmd = Serial.readString();
    Serial.flush();
    
    parseString(cmd); // call to function to parse string

    Serial.println(age);
    Serial.println(mode);
  } else {
    if(mode.equals("fitnessMode")){
      fitnessMode();
    } else if (mode.equals("stressMode")){
      Serial.println("stress mode");
    } else if (mode.equals("meditationMode")){
      Serial.println("meditation mode");
    }
  }
  delay(10);
}

// function which parses the incomming data
void parseString(String s){
  int delimitIndex = s.indexOf(','); // get the index of the comma

  // set the age and mode parameters
  age = s.substring(0, delimitIndex).toInt();
  mode = s.substring(delimitIndex+1);
}

void fitnessMode(){

  bRaw = analogRead(A1);
  Breath.Filter(bRaw);

  //!!!!!!!!!!!!!!!!!!==========================!!!!!!!!!!!!!!!!!!!===========================!!!!!!!!!!!!
 
  // if heart beat is sensed then print out the heartbeat
//  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
//    Serial.print(-1);
//    Serial.println(",");
//  }
//  else{
//    // send the value of analog input 0:
//    Serial.print(analogRead(A0));
//    Serial.println(",");
//  }

  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    //hRaw = analogRead(A0);
    //Heart.Filter(hRaw);
    Serial.print(0);
  } else {
    Serial.print(analogRead(A0));
  }
  //hh = Heart.Current();

  // Serial.print(hRaw);
  Serial.print(",");
  Serial.println(bRaw);

  setTime(0,0,0)

  if(val > threshold && !aboveThreshold)
  {
     counter++;
     aboveThreshold = true;
  }

  if(val < threshold)
    aboveThreshold = false;
}

bpm = counter/second();

if (counter > 60){
  counter = 0;
}

//    ecg=map(analogRead(A0), 0, 1000, 0, 100); 
//  if ((hh >= 250)&&(flag==0)) {
//    r1=millis();
//    flag=1;  
//  }
 
//  if ((hh >= 250)&&(flag==1)){
//    r2=millis();
//    flag=0;
//    flag_r=1;
//  }
 
//  if(flag_r==1){
//    BPM=6000/(r2-r1);
//    flag_r=0;
//  }

 Serial.print(",");
//  Serial.print(ecg);
 Serial.print(","); 
 Serial.print(bpm);  
 Serial.print ("\n"); 
  delay(10);
 
}
