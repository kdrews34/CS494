/*     PulseSensor Amped HRV Poincare Plotter v1.1.0
This is an HRV visualizer code for Pulse Sensor.
Use this with PulseSensorAmped_Arduino_1.5.0 Arduino code and the Pulse Sensor Amped hardware.
This code will draw a Poincare Plot of the IBI (InterBeat Interval) passed from Arduino.
The Poincare method of visualizing HRV trends is to plot the current IBI against the last IBI.
key press commands included in this version:
  press 'S' to save a picture of the Processing window (JPG image saved in Sketch folder)
  press 'C' to clear the Poincare Plot
  press 'L' to trace a line through the last 20 data points
Created by Joel Murphy, Early 2013.
Updated Summer 2013 for efficiency and readability
This code released into the public domain without promises that it will work for your intended application.
Or that it will work at all, for that matter. I hereby disclaim.
*/

import processing.serial.*;  // Serial library makes it possible to talk to Arduino
PFont font;                  // we will use text in this sketch
Serial port;                 // instantiate the Serial port

int IBI;                  // length of time between heartbeats in milliseconds (updated in serialEvent)
int[] PPG;                // array of live PPG datapoints
int[] beatTimeX;          // array of X coordinates of Poincare Plot
int[] beatTimeY;          // array of Y coordinates of Poincare Plot
int numPoints = 50;      // size of coordinate arrays. ths sets number of displayed datapoints
color eggshell = color(255, 253, 248);
// initializing flags here
boolean pulse = false;    // pulse is made true in serialEvent when arduino sends new IBI value
boolean makeLine = false; // press 'L' to toggle a trace the last 20 dots on the Poincare Plot

// SERIAL PORT STUFF TO HELP YOU FIND THE CORRECT SERIAL PORT
String serialPort = "COM3";

void setup() {                     // do all the sett'n up in the setup
  size(800,650);                     // Stage size
  beatTimeX = new int[numPoints];    // these two arrays hold the Poincare Plot data
  beatTimeY = new int[numPoints];    // size of numPoints determines number of displayed points
  PPG = new int[150];                // PPG array that that prints heartbeat waveform
  // initialize data traces
  resetDataTraces();
  
  textAlign(CENTER);                 // text and shapes will be referenced to their center point
  rectMode(CENTER);
  
  background(0);
  port = new Serial(this, serialPort, 19200);
  //port.bufferUntil('\n');
  
  drawDataWindows();

  // GO FIND THE ARDUINO
  fill(eggshell);
  delay(1000);
}  // END OF SETUP


void draw(){

   background(0);
//  DRAW THE BACKGROUND ELEMENTS AND TEXT
   drawDataWindows();
   writeAxisLabels();


//    DRAW THE POINCARE PLOT
  if (pulse == true && IBI != beatTimeX[0]){                    // check for new data from arduino
    pulse = false;                       // drop the pulse flag. it gets set in serialEvent
    for (int i=numPoints-1; i>0; i--){   // shift the data in n and n-1 arrays
      beatTimeY[i] = beatTimeY[i-1];
      beatTimeX[i] = beatTimeX[i-1];     // shift the data point through the array
    }
      beatTimeY[0] = beatTimeX[1];       // toss the old n into the n-1 spot
      beatTimeX[0] = IBI;                // update n with the current IBI value
    }

  fill(0,0,255);                         //  draw a history of the data points as blue dots
  for (int i=1; i<numPoints; i++){
    beatTimeX[i] = constrain(beatTimeX[i],0,1500);  // keep the values from escaping the Plot window!
    beatTimeY[i] = constrain(beatTimeY[i],0,1500);
    float  x = map(beatTimeX[i],0,1500,75,600);  // scale the data to fit the screen
    float  y = map(beatTimeY[i],0,1500,615,25);  // invert Y so it looks normal
    ellipse(x,y,5,5);                            // print datapoints as dots 2 pixel diameter
 }
   fill(250,0,0);                               // draw the most recent data point as a red dot
   float  x = map(beatTimeX[0],0,1500,75,600);  // scale the data to fit the screen
   float  y = map(beatTimeY[0],0,1500,615,25);  // invert Y so it looks normal
   ellipse(x,y,9,9);                            // print datapoint as a dot 5 pixel diameter
   fill(255,253,248);                           // eggshell white
   text("n: "+IBI+"mS",width-85,50);            // print the latest IBI value

//  TRACE THE LAST 20 DATAPOINTS IF THE OPTION IS SELECTED
  if(makeLine){                                         // toggle the makeLine flag by pressing 'L'
  stroke(0,0,255);                                      // trace the points in blue line
  noFill();
  beginShape();
  for (int i=0; i<20; i++){                             // trace the arc of n/n-1 for the last 20 points
    if(beatTimeX[i] == 0 || beatTimeY[i] == 0){break;}  // this solves for small data sets or long lines
    x = map(beatTimeX[i],0,1500,75,600);                // scale the data to fit the screen
    y = map(beatTimeY[i],0,1500,615,25);                // invert Y so it looks normal
    vertex(x,y);                                        // set the vertex coordinates
  }
  endShape();                                           // connect the vertices
  }

//   GRAPH THE PULSE SENSOR DATA
 stroke(250,0,0);                                       // use red for the pulse wave
  beginShape();                                         // beginShape is a fast way to draw lines!
  for (int i=1; i<PPG.length-1; i++){                   // scroll through the PPG array
    x = width-160+i;
    y = PPG[i];
    vertex(x,y);                                        // set the vertex coordinates
  }
  endShape();                                           // connect the vertices
  noStroke();
}  //END OF DRAW

void drawDataWindows(){
  noStroke();
  fill(eggshell);
  rect(width/2-50,height/2+15,550,550);     // draw Poincare Plot window
  rect(width-85,(height/2)+15,150,550);     // draw the Pulse Sensor data window

}

void writeAxisLabels(){
  noStroke();
  fill(eggshell);                        // eggshell white
  textSize(20);
  text("Pulse Sensor HRV Poincare Plot",width/2-50,30);  // title
  
  //Calculate HRV
  int sum = 0;
  int HRV;
  for (int i = 0; i < numPoints-1; i++){
    sum += pow(beatTimeX[i+1] - beatTimeX[i], 2);
  }
  HRV = (int)sqrt(sum / (numPoints-1));
  text("HRV: " + HRV + "ms", width/2-50, 55);
  
  fill(200);                                // draw the Plot coordinate values in grey
  text("0mS",40,height-25);                 // origin, scaled in mS
  for (int i=500; i<=1500; i+=500){         // print x axis values
    text(i, 40,map(i,0,1500,615,75));
  }
  for (int i=500; i<=1500; i+=500){         // print  Y axis values
    text(i, 75+map(i,0,1500,0,550), height-10);
  }
  stroke(250,30,250);                       // draw gridlines in purple
  for (int i=0; i<1500; i+=100){            // draw grid lines on axes
    line(75,map(i,0,1500,614,26),85,map(i,0,1500,614,26)); //y axis
    line(75+map(i,0,1500,0,549),height-35,75+map(i,0,1500,0,549),height-45); // x axis
  }
  noStroke();
  fill(255,253,10);                                    // print axes legend in yellow, for fun
  text("n", 75+map(750,0,1500, 0, 550), height-10);    // n is the most recent IBI value
  text("n-1",40,map(750,0,1500,615,75));               // n-1 is the one we got before n
}

void resetDataTraces(){
  // initialize the PPG
  for (int i=0; i<150; i++){
   PPG[i] = height/2+15;             // initialize PPG widow with dataline at midpoint
  }
  // initialize the poncaire points
  for (int i=numPoints-1; i>=0; i--){  //
    beatTimeY[i] = 0;
    beatTimeX[i] = 0;
  }
}

void serialEvent(Serial port){
  try{
    String inData = port.readStringUntil('\n');
    
    if(inData == null) {println("not getting data"); return; }
    
    String[] vals = split(inData, ',');
    int newPPG = int(vals[0]);

    for (int i = 0; i < PPG.length-1; i++){
        PPG[i] = PPG[i+1];
    }

    PPG[PPG.length-1] = int(map(newPPG,50,900,(height/2+15)+225,(height/2+15)-225));

    IBI = Integer.parseInt(vals[1]);
    pulse = true;
  }
  catch(Exception e){
    println(e);
  }
}
