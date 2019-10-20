
int UpperThreshold = 700;
int LowerThreshold = 680;
int stepThresh = 123;
int FSRMF =0;
int FSRLF =0;
int FSRMM =0;
int FSRh = 0;
int Cadence = 0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned int FirstPulseTime = 0;
unsigned int SecondPulseTime = 0;
unsigned int Time = 0;
unsigned int ts = 0;
int stride = 0;
int stepl = 0;
float spd = 0;
int stepcount = 0;


void setup() {
  // put your setup code here, to run once:
// initialize the serial communication:
  Serial.begin(19200);
  Serial.flush(); // clear receiver buffer
   pinMode(3, OUTPUT); // initializing LEDs
   pinMode(5, OUTPUT); 
   pinMode(9, OUTPUT); 
   pinMode(11, OUTPUT); 

}

void loop() {
  // put your main code here, to run repeatedly:
FSRMF = analogRead(A0);
FSRLF = analogRead(A1);
FSRMM = analogRead(A2);
FSRh = analogRead(A3);

  
  //CODE FOR STEP TIME AND RELATED VALUES
  if(FSRh > UpperThreshold && IgnoreReading == false){
    if(FirstPulseDetected == false){
      FirstPulseTime = millis();
      FirstPulseDetected = true;
    }
    else{
      SecondPulseTime = millis();
      Time = SecondPulseTime - FirstPulseTime;
      FirstPulseTime = SecondPulseTime;
    }
    IgnoreReading = true;
  }

  // Heart beat trailing edge detected.
  if(FSRh < LowerThreshold){
    IgnoreReading = false;
} 
ts = Time/1000;
stride = .65*ts;
stepl = stride/2;
Cadence = (1/Time) * 60 * 1000;
spd = stride/ts;

if(FSRMF>stepThresh){
  digitalWrite(3,HIGH);
}else{
  digitalWrite(3,LOW);
}

if(FSRLF>stepThresh){
  digitalWrite(5,HIGH);
}else{
  digitalWrite(5,LOW);
}

if(FSRMM>stepThresh){
  digitalWrite(9,HIGH);
}else{
  digitalWrite(9,LOW);
}

if(FSRh>stepThresh){
  stepcount = stepcount+1;
  digitalWrite(11,HIGH);
}else{
  digitalWrite(11,LOW);
}

Serial.print(stepl);
Serial.print(",");
Serial.print(stride);
Serial.print(",");
Serial.print(Cadence);
Serial.print(",");
Serial.print(spd);
Serial.print(",");
Serial.println(stepcount);

}

