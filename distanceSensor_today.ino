#include "MegunoLink.h"
#include "Filter.h"
#include "PulseSensorPlayground.h"

// declare global variables
String cmd;
int age;
String mode;

// filter for breath
ExponentialFilter<float> Breath(10,0);
int bRaw;

int BPM = 0;
float r1=0;
float r2=0;
int flag=0;
int flag_r=0;
float ecg;
int hh = 0;

int buzzer = 2;

void setup() {
  // initialize the serial communication:
  Serial.begin(19200);
  Serial.flush(); // clear receive buffer
  pinMode(11, INPUT); // Setup for leads off detection LO +
  pinMode(10, INPUT); // Setup for leads off detection LO -
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // if there is incoming from the serial read it
//  if(Serial.available()){
//    cmd = Serial.readString();
//    Serial.flush();
//    
//    parseString(cmd); // call to function to parse string
//
//    Serial.println(age);
//    Serial.println(mode);
//  } else {
//    if(mode.equals("fitnessMode")){
//      fitnessMode();
//    } else if (mode.equals("stressMode")){
//      Serial.println("stress mode");
//    } else if (mode.equals("meditationMode")){
//      Serial.println("meditation mode");
//    }
//  }
//  delay(10);

  unsigned char i,j;

  while(1){
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
    delay(1000);
  }
  while(1)
  {
    //output an frequency
    for(i=0;i<80;i++)
    {
      digitalWrite(buzzer,HIGH);
      delay(1);//wait for 1ms
      digitalWrite(buzzer,LOW);
      delay(1);//wait for 1ms
    }
    //output another frequency
    for(i=0;i<100;i++)
    {
      digitalWrite(buzzer,HIGH);
      delay(2);//wait for 2ms
      digitalWrite(buzzer,LOW);
      delay(2);//wait for 2ms
    }
  }
}

// function which parses the incomming data
void parseString(String s){
  int delimitIndex = s.indexOf(','); // get the index of the comma

  // set the age and mode parameters
  age = s.substring(0, delimitIndex).toInt();
  mode = s.substring(delimitIndex+1);
}

void fitnessMode(){

  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    Serial.print(0);
  } else {
    Serial.print(analogRead(A0));
  }

  Serial.print(",");

  // filter the breath rate and send to the serial
  bRaw = analogRead(A4);
  Breath.Filter(bRaw);
  Serial.println(Breath.Current());

  if ((hh >= 900)&&(flag==0)) {
    r1=millis();
    flag=1;  
    delay(80);
  }
  
  if ((hh >= 900)&&(flag==1)){
    r2=millis();
    flag=0;
    flag_r=1;
  }
  
  if (flag_r==1){
    BPM=6000/(r2-r1);
    flag_r=0;
  }
}
