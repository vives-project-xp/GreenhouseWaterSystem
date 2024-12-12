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

/*
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
*/

/*!
 * @file readVoltage.ino
 * @brief connect ADS1115 I2C interface with your board (please reference board compatibility)
 * @n The voltage value read by A0 A1 A2 A3 is printed through the serial port.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [luoyufeng](yufeng.luo@dfrobot.com),
 * @version  V1.0
 * @date  2019-06-19
 * @url https://github.com/DFRobot/DFRobot_ADS1115
 */

/*
#include <Wire.h>
#include <DFRobot_ADS1115.h>

DFRobot_ADS1115 ads(&Wire);

void setup(void) 
{
    Serial.begin(115200);
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x48
    ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
    ads.setMode(eMODE_SINGLE);       // single-shot mode
    ads.setRate(eRATE_128);          // 128SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
    ads.init();
}

void loop(void) 
{
    if (ads.checkADS1115())
    {
        int16_t adc0, adc1, adc2, adc3;
        adc0 = ads.readVoltage(0);
        Serial.print("A0:");
        Serial.print(adc0);
        Serial.print("mV,  ");
        adc1 = ads.readVoltage(1);
        Serial.print("A1:");
        Serial.print(adc1);
        Serial.print("mV,  ");
        adc2 = ads.readVoltage(2);
        Serial.print("A2:");
        Serial.print(adc2);
        Serial.print("mV,  ");
        adc3 = ads.readVoltage(3);
        Serial.print("A3:");
        Serial.print(adc3);
        Serial.println("mV");
    }
    else
    {
        Serial.println("ADS1115 Disconnected!");
    }

    delay(1000);
}

*/
/*
#include "Arduino.h"
#include <Wire.h>
#include <DFRobot_ADS1115.h>
#include "DFRobot_ESP_EC.h"
#include "EEPROM.h"

DFRobot_ESP_EC ec;
DFRobot_ADS1115 ads(&Wire);

float voltage, ecValue, temperature = 25;
float calibration_value = 21.34 + 0.7; // Calibration value for pH adjustment
unsigned long int avgValue;            // Store the average value of the sensor feedback
int buf[10], temp;

void setup()
{
	Serial.begin(115200);
	EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
	ec.begin();//by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30
  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x48
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);       // single-shot mode
  ads.setRate(eRATE_128);          // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads.init();
}

void loop()
{
  /*
	static unsigned long timepoint = millis();
	if (millis() - timepoint > 1000U) //time interval: 1s
	{

		timepoint = millis();
		voltage = ads.readVoltage(0) / 10;
		Serial.print("voltage:");
		Serial.println(voltage, 4);

		//temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
		Serial.print("temperature:");
		Serial.print(temperature, 1);
		Serial.println("^C");

		ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
		Serial.print("EC:");
		Serial.print(ecValue, 4);
		Serial.println("ms/cm");
	}
	ec.calibration(voltage, temperature); // calibration process by Serail CMD
  


  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read the input line
    Serial.print("You sent: ");
    Serial.println(input); // Echo the input back
  }


}

float readTemperature()
{
	//add your code here to get the temperature from your temperature sensor
}
*/
#include "Arduino.h"
#include <Wire.h>
#include <DFRobot_ADS1115.h>
#include "DFRobot_ESP_EC.h"
#include "EEPROM.h"

// Create instances for the EC sensor and ADS1115 ADC
DFRobot_ESP_EC ec;
DFRobot_ADS1115 ads(&Wire);

float voltage, ecValue, temperature = 25.0; // Default temperature is 25°C

void setup()
{
    Serial.begin(115200);
    EEPROM.begin(32); // Needed for storing calibration K in EEPROM
    ec.begin();       // Initialize EC sensor library (default calibration index is 10)

    // ADS1115 initialization
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1); // I2C address: 0x48
    ads.setGain(eGAIN_TWOTHIRDS);              // Set ADC gain to 2/3 (default)
    ads.setMode(eMODE_SINGLE);                // Single-shot mode
    ads.setRate(eRATE_128);                   // Data rate: 128 SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);            // Start single conversion
    ads.init();                               // Initialize the ADC
}

void loop()
{
    // Static variable to manage timing for periodic measurement
    static unsigned long timepoint = millis();

    if (millis() - timepoint > 1000U) // Measure every 1 second
    {
        timepoint = millis();

        // Read the voltage from channel 0 of ADS1115
        voltage = ads.readVoltage(0) / 2; // Convert millivolts to volts (ADC returns mV)

        // TODO: Replace this line with a temperature reading function if using a temperature sensor
        // temperature = readTemperature(); // Read temperature sensor (if connected)

        // Compute EC value with temperature compensation
        ecValue = ec.readEC(voltage, temperature);

        // Print the results
        Serial.print("Temperature: ");
        Serial.print(temperature, 1); // Print temperature with 1 decimal place
        Serial.print("°C  EC: ");
        Serial.print(ecValue, 2);     // Print EC value with 2 decimal places
        Serial.println(" ms/cm");
    }

    // Allow calibration via serial command
    ec.calibration(voltage, temperature);
}
