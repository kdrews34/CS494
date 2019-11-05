//lab 2 arduion code

int LED = 5;
unsigned char i,j;
int UpperThreshold = 550;
int LowerThreshold = 480;
int hReading = 0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
int PulseInterval = 0;

void setup() {
  // initialize the serial communication:
  Serial.begin(19200);
  Serial.flush(); // clear receiver buffer
  pinMode(LED, OUTPUT);
}

void loop() {
    hReading = analogRead(A0);
    Serial.print(hReading);

    //Turn LED on if heartRate is above threshold
    if (hReading >= UpperThreshold)
        digitalWrite(LED, HIGH);
    else
        digitalWrite(LED,LOW);
  
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

    Serial.print(",");
    Serial.print((int)PulseInterval);
    Serial.println(",0");
}