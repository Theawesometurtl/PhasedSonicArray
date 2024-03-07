const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
int const pin0 = A0;       // Preamp output pin connected to A0
int const pin1 = A1;       // Preamp output pin connected to A0
int const pin2 = A2;       // Preamp output pin connected to A0
int const pin3 = A3;       // Preamp output pin connected to A0
int const pin4 = A4;       // Preamp output pin connected to A0
int const pin5 = A5;

int const PIN_AMOUNT = 6;
int r = 0;

const int NUM_READINGS = 100;

int const pins[PIN_AMOUNT] = {pin0, pin1, pin2, pin3, pin4, pin5};
int  calibration[PIN_AMOUNT] = {0, 0, 0, 0, 0, 0};
String const pinAngle[PIN_AMOUNT] = {"-3 Degrees", "-2 Degrees", "-1 Degrees", "1 Degrees", "2 Degrees", "3 Degrees"};
unsigned int sample[PIN_AMOUNT] = {0, 0, 0, 0, 0, 0};
String myString = "";


#include <SD.h>
#define BMP280_ADDRESS 0x76

int compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // collect data for 50 mS and then plot data
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak[PIN_AMOUNT] = {0, 0, 0, 0, 0, 0};   // peak-to-peak level
  unsigned int p[PIN_AMOUNT];   // peak-to-peak level


  unsigned int signalMax[PIN_AMOUNT] = {0, 0, 0, 0, 0, 0};

  unsigned int signalMin[PIN_AMOUNT] = {1024, 1024, 1024, 1024, 1024, 1024};


  // collect data for 50 mS and then plot data
  while (millis() - startMillis < sampleWindow)
  {
    for (int i=0; i<PIN_AMOUNT; i++) {
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
  for (int i=0; i<PIN_AMOUNT; i++) {
    peakToPeak[i] = signalMax[i] - signalMin[i];  // max - min = peak-peak amplitude
    if (r >= NUM_READINGS) {
      peakToPeak[i] -= calibration[i];
    }
    myString+= String(peakToPeak[i]);
    myString+= " ";
    p[i] = peakToPeak[i];
  }




  Serial.print(myString);

  qsort(p, PIN_AMOUNT, sizeof(p[0]), compare);

  if (p[0] != p[1]) {
    for (int i=0; i<PIN_AMOUNT; i++) {
      if (p[0] == peakToPeak[i]) {
        // Serial.print(" ");
        Serial.print(pinAngle[i]);
      }
    }
  }

  Serial.println();


  if (r < NUM_READINGS) {
    r++;
    for (int i=0; i<PIN_AMOUNT; i++) {
      calibration[i] += peakToPeak[i] / NUM_READINGS;
    }

  }
}

