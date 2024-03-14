const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
int const pin0 = A0;       //input analog pins for the microphones
int const pin1 = A1;       
int const pin2 = A2;       
int const pin3 = A3;       
int const pin4 = A4;       
int const pin5 = A5;
int const pin6 = A6;

int const PIN_AMOUNT = 7; // the amount of microphones in your array
bool RUNNING = true; // when this is false, calculations don't run
int r = 0; // this variable grows during calibration. 

int const averageReadingDuration = 10; // these variables are for finding the microphone that consistently was the loudest over a time period of 10 updates
int previousHighest[averageReadingDuration];
int previousHighestCopy[averageReadingDuration];
int average = 0;
int a = 0;
int a2 = 0;

const int NUM_READINGS = 100; // this is the number of readings used in calibration

int const pins[PIN_AMOUNT] = {pin0, pin1, pin2, pin3, pin4, pin5, pin6};
unsigned int p[PIN_AMOUNT];
unsigned int  calibration[PIN_AMOUNT] = {0, 0, 0, 0, 0, 0, 0};
int const pinAngle[PIN_AMOUNT] = {0, 30, 60,// set these strings to the angle of each sensor in the microphone array,
       90, 120, 150, 180};            // starting with the microphone on A0
                                                                  
unsigned int sample[PIN_AMOUNT];
unsigned int peakToPeak[PIN_AMOUNT];
unsigned int signalMax[PIN_AMOUNT] ;
unsigned int signalMin[PIN_AMOUNT] ;
String myString = "";


#include <SD.h>
#define BMP280_ADDRESS 0x76
#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.

SoftwareSerial SUART(2, 3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 

int findMostCommon() { // This function tells you the mode of an array
  for (int i = 0; i < averageReadingDuration; i++) {
    previousHighestCopy[i] = previousHighest[i];
  }
  qsort(previousHighestCopy, averageReadingDuration, sizeof(previousHighestCopy[0]), compare);//sorting previousHighest list

  int modeCt = 0;
  int modeV = -1;

  int ct = 0;
  int v = -1;

  for(int i = 0; i< averageReadingDuration; i++) {
    
    if(previousHighestCopy[i] != v) {
      v = previousHighestCopy[i];
      ct = 0;
    }

    ct ++;

    if(ct > modeCt) {
      modeCt = ct;
      modeV = v;
    }
  }
  return modeV;
}

int findWeightedAverage() {
  int long unweightedSum = 0;
  int long sumOfValues = 0;
  for(int i = 0; i< PIN_AMOUNT; i++) {
    unweightedSum += peakToPeak[i] * pinAngle[i];

    sumOfValues += peakToPeak[i];
  }

  int answer = unweightedSum / sumOfValues;


  return answer;
}

int compare(const void *a, const void *b) { 
    return (*(int *)b - *(int *)a);
}


void setup()
{
  Serial.begin(9600);
  SUART.begin(4800);

}


int findLargest() { // this function finds the largest integer in a list
  qsort(p, PIN_AMOUNT, sizeof(p[0]), compare);

  if (p[0] != p[1]) {
    for (int i=0; i<PIN_AMOUNT; i++) {
      if (p[0] == peakToPeak[i]) {
        // Serial.print(" ");
        // Serial.print(pinAngle[i]);
        return i;
      }
    }
  }
  // return 0;
}

int calculateVolume(int i) { // Calculates the sound level of a sensor. Much of the code is inspired by this website: https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels
      sample[i] = analogRead(pins[i]);
      if (sample[i] < 1024) {
        if (sample[i] > signalMax[i]) {
          signalMax[i] = sample[i];
        } else if (sample[i] < signalMin[i]) {
          signalMin[i] = sample[i];
        }
      }
    
  
  myString = "";
  for (int i=0; i<PIN_AMOUNT; i++) {
    peakToPeak[i] = signalMax[i] - signalMin[i];  // max - min = peak-peak amplitude
    if (r >= NUM_READINGS && peakToPeak[i]*100 / calibration[i] != 65535) {
      // peakToPeak[i] = peakToPeak[i]*100 / calibration[i];
    }
    // myString+= (String(peakToPeak[i]) + "      ").substring(0,4);// This appends the microphone data to a string that will later be sent to the serial
    myString+= peakToPeak[i];// This appends the microphone data to a string that will later be sent to the serial
                                                                // monitor. Each string is the same length, so it's easier to read the values on the serial monitor
    myString+= " ";
    
    p[i] = peakToPeak[i]; 
  }
  return peakToPeak;
}



void calibrateSensors() {// ran every frame during calibration period until r > NUM_READINGS
  r++;
  for (int i=0; i<PIN_AMOUNT; i++) {
    calibration[i] += peakToPeak[i] / NUM_READINGS;
  }
}

void loop()
{
  if (RUNNING) {
  // collect data for 50 mS and then plot data
  unsigned long startMillis = millis(); // Start of sample window

  for (int i=0; i<PIN_AMOUNT; i++) {
    peakToPeak[i] = 0;   // peak-to-peak level
    signalMax[i] = 0;
    signalMin[i] = 1024;
  }
  // collect data for 50 mS and then plot data
  while (millis() - startMillis < sampleWindow)//this loop does all the sound level calculations
  {
    for (int i=0; i<PIN_AMOUNT; i++) {
      peakToPeak[i] = calculateVolume(i); 
    }
  }


  Serial.print(myString);
  a = findLargest();
  Serial.println();
  Serial.print(average);
  Serial.print("° ");
  // Serial.print(" ");
  // Serial.print(average);
  // Serial.print(" ");
  // Serial.print(a);



  if (r < NUM_READINGS) {
    calibrateSensors();
  }
  }
  SUART.listen(); // listening on Serial One
  
  while (SUART.available() > 0) {//optional code if you want to connect up multiple arduinos together using software serial
    char inByte = SUART.read();
    // Serial.write(inByte);
    Serial.print(String(inByte));
    if (String(inByte) == "Start") {
      RUNNING = true;
    }
    if (String(inByte) == "End") {
      RUNNING = false;
    }
    if (String(inByte) == "Calibrate") {
      r = 0;
    }
  }

  previousHighest[a2] = a;
  a2++;
  if (a2 > averageReadingDuration-1) {
    a2 = 0;
  }
  average = findMostCommon();
  
  
}