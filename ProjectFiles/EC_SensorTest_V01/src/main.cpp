//EC sensor calibration
/*
#include <Arduino.h>
#include <DFRobot_EC.h>
#include <EEPROM.h>

#define EC_PIN 1
float voltage,ecValue,temperature = 25;
DFRobot_EC ec;

void setup()
{
  Serial.begin(115200);  
  ec.begin();
  pinMode(EC_PIN,INPUT);
}

void loop()
{
    static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U)  //time interval: 1s
    {
      timepoint = millis();
      voltage = analogRead(EC_PIN)/4095.0 * 3300;  // read the voltage
      ecValue =  ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
      Serial.print("temperature:");
      Serial.print(temperature,1);
      Serial.print("^C  EC:");
      Serial.print(ecValue,2);
      Serial.println("ms/cm");
    }
    ec.calibration(voltage,temperature);          // calibration process by Serail CMD
}
*/

// PH sensor calibration
#include <Arduino.h>
#define SensorPin A0      // pH meter Analog output to Arduino Analog Input 0
float calibration_value = 21.34 + 0.7; // Calibration value for pH adjustment
unsigned long int avgValue;            // Store the average value of the sensor feedback
int buf[10], temp;

void setup() {
  pinMode(SensorPin, INPUT);
  Serial.begin(9600);
  delay(2000); 
}

void loop() {
  for(int i = 0; i < 10; i++) {       // Get 10 sample values from the sensor for smoothing
    buf[i] = analogRead(SensorPin);
    delay(10);
  }

  // Sort the analog readings from small to large
  for(int i = 0; i < 9; i++) {
    for(int j = i + 1; j < 10; j++) {
      if(buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  avgValue = 0;
  for(int i = 2; i < 8; i++) avgValue += buf[i];  // Take the average of the 6 center samples

  // Convert the analog reading to voltage (assuming a 3.3V reference)
  float phVoltage = (float)avgValue * 3.3 / 1024 / 6; 
  
  // Convert the voltage to a pH value
  float phValue = -5.70 * phVoltage + calibration_value;

  // Display pH level as text
  if(phValue < 4)                   Serial.println("Very Acidic");
  else if(phValue >= 4 && phValue < 5)  Serial.println("Acidic");
  else if(phValue >= 5 && phValue < 7)  Serial.println("Acidic-ish");
  else if(phValue >= 7 && phValue < 8)  Serial.println("Neutral");
  else if(phValue >= 8 && phValue < 10) Serial.println("Alkaline-ish");
  else if(phValue >= 10 && phValue < 11) Serial.println("Alkaline");
  else if(phValue >= 11)                Serial.println("Very Alkaline");

  delay(1000);
}
