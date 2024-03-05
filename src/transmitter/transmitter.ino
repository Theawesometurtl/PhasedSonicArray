const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
int const pin0 = A0;       // Preamp output pin connected to A0
int const pin1 = A1;       // Preamp output pin connected to A0
int const pin2 = A2;       // Preamp output pin connected to A0
int const pin3 = A3;       // Preamp output pin connected to A0
int const pin4 = A4;       // Preamp output pin connected to A0

unsigned int sample0;
unsigned int sample1;
unsigned int sample2;
unsigned int sample3;
unsigned int sample4;

bool transmitting = true;


#define BMP280_ADDRESS 0x76
#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.

SoftwareSerial SUART(2, 3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 
char myData[10] = "";       // Creates a blank character array of size 10
int i = 0;
int z;




void setup()
{
  Serial.begin(9600);
  SUART.begin(4800);
}

void loop()
{
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak0 = 0;   // peak-to-peak level
  unsigned int peakToPeak1 = 0;   // peak-to-peak level
  unsigned int peakToPeak2 = 0;   // peak-to-peak level
  unsigned int peakToPeak3 = 0;   // peak-to-peak level
  unsigned int peakToPeak4 = 0;   // peak-to-peak level

  unsigned int signalMax0 = 0;
  unsigned int signalMax1 = 0;
  unsigned int signalMax2 = 0;
  unsigned int signalMax3 = 0;
  unsigned int signalMax4 = 0;
  unsigned int signalMin0 = 1024;
  unsigned int signalMin1 = 1024;
  unsigned int signalMin2 = 1024;
  unsigned int signalMin3 = 1024;
  unsigned int signalMin4 = 1024;

  // collect data for 50 mS and then plot data
  while (millis() - startMillis < sampleWindow)
  {
    sample0 = analogRead(pin0);
    sample1 = analogRead(pin1);
    sample2 = analogRead(pin2);
    sample3 = analogRead(pin3);
    sample4 = analogRead(pin4);
    if (sample0 < 1024)  // toss out spurious readings
    {
      if (sample0 > signalMax0)
      {
        signalMax0 = sample0;  // save just the max levels
      }
      else if (sample0 < signalMin0)
      {
        signalMin0 = sample0;  // save just the min levels
      }
    }
    if (sample1 < 1024)  // toss out spurious readings
    {
      if (sample1 > signalMax1)
      {
        signalMax1 = sample1;  // save just the max levels
      }
      else if (sample1 < signalMin1)
      {
        signalMin1 = sample1;  // save just the min levels
      }
    }
    if (sample2 < 1024)  // toss out spurious readings
    {
      if (sample2 > signalMax2)
      {
        signalMax2 = sample2;  // save just the max levels
      }
      else if (sample2 < signalMin2)
      {
        signalMin2 = sample2;  // save just the min levels
      }
    }
    if (sample3 < 1024)  // toss out spurious readings
    {
      if (sample3 > signalMax3)
      {
        signalMax3 = sample3;  // save just the max levels
      }
      else if (sample3 < signalMin3)
      {
        signalMin3 = sample3;  // save just the min levels
      }
    }
    if (sample4 < 1024)  // toss out spurious readings
    {
      if (sample4 > signalMax4)
      {
        signalMax4 = sample4;  // save just the max levels
      }
      else if (sample4 < signalMin4)
      {
        signalMin4 = sample4;  // save just the min levels
      }
    }
  }
  peakToPeak0 = signalMax0 - signalMin0;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak0);
  Serial.print("   ");
  peakToPeak1 = signalMax1 - signalMin1;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak1);
  Serial.print("   ");
  peakToPeak2 = signalMax2 - signalMin2;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak2);
  Serial.print("   ");
  peakToPeak3 = signalMax3 - signalMin3;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak3);
  Serial.print("   ");
  peakToPeak4 = signalMax4 - signalMin4;  // max - min = peak-peak amplitude
  Serial.println(peakToPeak4);
  //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  //Serial.println(volts);

  // Serial.println(data);//write(Data);
  if (transmitting) {
    SUART.print(peakToPeak0);//write(Data);
    SUART.println();   //Newline code (0x0A) as end mark
    delay(100);
    SUART.print(peakToPeak1);//write(Data);
    SUART.println();   //Newline code (0x0A) as end mark
    delay(100);
    SUART.print(peakToPeak2);//write(Data);
    SUART.println();   //Newline code (0x0A) as end mark
    delay(100);
    SUART.print(peakToPeak3);//write(Data);
    SUART.println();   //Newline code (0x0A) as end mark
    delay(100);
    SUART.print(peakToPeak4);//write(Data);
    SUART.println();   //Newline code (0x0A) as end mark
    delay(100);
  }
}
