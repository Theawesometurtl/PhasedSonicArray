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


#include <SD.h>
#define BMP280_ADDRESS 0x76
#include<SoftwareSerial.h>  // The library to create a secondary serial monitor on arduino uno.

SoftwareSerial SUART(2, 3); // Sets the input and output ports to Digital Pins 3 and 4. They should be reversed with the pins on the speedometer. 
char myData[55] = "";       // Creates a blank character array of size 10
int i = 0;
int f = 0;
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
  Serial.print("  ");
  peakToPeak1 = signalMax1 - signalMin1;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak1);
  Serial.print("  ");
  peakToPeak2 = signalMax2 - signalMin2;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak2);
  Serial.print("  ");
  peakToPeak3 = signalMax3 - signalMin3;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak3);
  Serial.print("  ");
  peakToPeak4 = signalMax4 - signalMin4;  // max - min = peak-peak amplitude
  Serial.print(peakToPeak4);
  Serial.print("  ");
  //double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  //Serial.println(volts);

    // put your main code here, to run repeatedly:
  // char x = SUART.read(); 
  // if (SUART.available()){
  //   if (x != 0x0A)  //end mark no found
  //   {
  //     myData[i] = x;  //save ASCII coded data in array
  //     i++;
  //   }
  //   else
  //   {
  //     z = atoi(myData);  		 // getting the data in integer form
  //     i = 0;
  //     Serial.println("end");
  //     while (f=0,f<=55,f++){
  //       myData[f]="";
  //     }
  //   }
  // // Serial.println(z);
  // Serial.println(myData);
  // Serial.print(SUART.available());
  
  SUART.listen(); // listening on Serial One

  // Serial.println("Data from port one:");
  while (SUART.available() > 0) {
    char inByte = SUART.read();
    Serial.write(inByte);
  }
  Serial.println();
  
}
