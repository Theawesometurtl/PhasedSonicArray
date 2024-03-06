const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
int const pin0 = A0;       // Preamp output pin connected to A0
int const pin1 = A1;       // Preamp output pin connected to A0
int const pin2 = A2;       // Preamp output pin connected to A0
int const pin3 = A3;       // Preamp output pin connected to A0
int const pin4 = A4;       // Preamp output pin connected to A0

int const pins[5] = {pin0, pin1, pin2, pin3, pin4};
unsigned int sample[5] = {0, 0, 0, 0, 0};
String myString = "";



#include <SD.h>
#define BMP280_ADDRESS 0x76
#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.

SoftwareSerial SUART(2, 3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 



void setup()
{
  Serial.begin(9600);
  SUART.begin(4800);
}

void loop()
{
  // collect data for 50 mS and then plot data
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak[5] = {0, 0, 0, 0, 0};   // peak-to-peak level


  unsigned int signalMax[5] = {0, 0, 0, 0, 0};

  unsigned int signalMin[5] = {1024, 1024, 1024, 1024, 1024};


  // collect data for 50 mS and then plot data
  while (millis() - startMillis < sampleWindow)
  {
    for (int i=0; i<5; i++) {
      sample[i] = analogRead(pins[i]);


      if (sample[i] < 1024) {
        if (sample[i] > signalMax[i]) {
          signalMax[i] = sample[i];
        } else if (sample[i] < signalMin[i]) {
          signalMin[i] = sample[i];
        }
      }
    }
  }

  myString = "";
  for (int i=0; i<5; i++) {
    peakToPeak[i] = signalMax[i] - signalMin[i];  // max - min = peak-peak amplitude
    myString+= String(peakToPeak[i]);
    myString+= "  ";
  }
  
  SUART.listen(); // listening on Serial One

  Serial.print(myString);
  while (SUART.available() > 0) {
    char inByte = SUART.read();
    Serial.write(inByte);
  }
  Serial.println();
}

