onst int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
int const pin0 = A0;       // Preamp output pin connected to A0
int const pin1 = A1;       // Preamp output pin connected to A0
int const pin2 = A2;       // Preamp output pin connected to A0
int const pin3 = A3;       // Preamp output pin connected to A0
int const pin4 = A4;       // Preamp output pin connected to A0
int const pin5 = A5;
int const pin6 = A6;

int const PIN_AMOUNT = 7;
bool RUNNING = true;
int r = 0;

int const averageReadingDuration = 10;
int average = 0;
int a = 0;
int a2 = 0;

const int NUM_READINGS = 100;

int const pins[PIN_AMOUNT] = {pin0, pin1, pin2, pin3, pin4, pin5, pin6};
unsigned int p[PIN_AMOUNT];
unsigned int  calibration[PIN_AMOUNT] = {0, 0, 0, 0, 0, 0, 0};
String const pinAngle[PIN_AMOUNT] = {"0 Degrees", "30 Degrees", "60 Degrees", "90 Degrees", "120 Degrees", "150 Degrees", "180 Degrees"};
unsigned int sample[PIN_AMOUNT];
unsigned int peakToPeak[PIN_AMOUNT];
unsigned int signalMax[PIN_AMOUNT] ;
unsigned int signalMin[PIN_AMOUNT] ;
String myString = "";





#include <SD.h>
#define BMP280_ADDRESS 0x76
#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.

SoftwareSerial SUART(2, 3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 


int compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}


void setup()
{
  Serial.begin(9600);
  SUART.begin(4800);

}


int findLargest() {
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

int calculateVolume(int i) {
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
    if (r >= NUM_READINGS) {
      peakToPeak[i] -= calibration[i];
    }
    myString+= String(peakToPeak[i]);
    myString+= " ";
    p[i] = peakToPeak[i];
  }
  return peakToPeak;
}



void calibrateSensors() {
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
  while (millis() - startMillis < sampleWindow)
  {
    for (int i=0; i<PIN_AMOUNT; i++) {
      peakToPeak[i] = calculateVolume(i);
    }
  }


  Serial.print(myString);
  a += findLargest();
  Serial.print(pinAngle[average]);
  // Serial.print(" ");
  // Serial.print(average);
  // Serial.print(" ");
  // Serial.print(a);
  // Serial.print(" ");
  // Serial.print(a2);
  Serial.println();


  if (r < NUM_READINGS) {
    calibrateSensors();
  }
  }
  SUART.listen(); // listening on Serial One
  
  while (SUART.available() > 0) {
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

  a2++;
  if (a2 > averageReadingDuration) {
    average = a /averageReadingDuration;
    average = round(average);
    a=0;
    a2 =0;
  }
}