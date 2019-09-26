#include "MegunoLink.h"
#include "Filter.h"
#include "Time.h"
#include "Timelib.h"

String cmd;
int age;
String mode;

ExponentialFilter<float> Breath(10,0);

int bRaw;
int hRaw;
bool aboveThreshold = false;
const int heartPin = 0;
int threshold = 500;
int counter = 0;
int bpm = 0

void setup() {
  // initialize the serial communication:
  Serial.begin(19200);
  Serial.flush(); // clear receive buffer
  pinMode(11, INPUT); // Setup for leads off detection LO +
  pinMode(10, INPUT); // Setup for leads off detection LO -
  setTime(0,0,0);
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

  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    Serial.print(0);
  } else {
    Serial.print(analogRead(A0)); //printing heart sensor value
  }

  Serial.print(","); //printing delimiter
  Serial.println(Breath.Current()); //printing breath sensor value

  if(val > threshold && !aboveThreshold)
  {
     counter++;
     aboveThreshold = true;
  }

  if(val < threshold)
    aboveThreshold = false;

    Serial.print(","); //printing delimiter
    Serial.print(bpm);  //printing bpm
    Serial.print ("\n"); 
}