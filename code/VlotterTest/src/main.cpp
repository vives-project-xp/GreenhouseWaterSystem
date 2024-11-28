#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>

#define sensor_Empty_reservoir_1 0 //sensor for 35 left
#define sensor_Low_reservoir_1  1 //sensor for 80 left
#define sensor_Mid_reservoir_1  2 //sensor for 165 left
#define sensor_High_reservoir_1 3 //sensor for 210 left
#define pH_sensor 4 //sensor for pH
#define relay 5 //relay for pump
#define sensor_Low_reservoir_0 6 //sensor for 25 left
#define sensor_Mid_reservoir_0  7 //sensor for 70 left
#define sensor_High_reservoir_0  8 //sensor for 185 left

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid = "devbit";
const char* password = "Dr@@dloos!";
const String serverURL = "http://10.10.2.70:3000"; // Replace with your server's IP

float calibration_value = 21.34 + 0.7;
unsigned long int avgValue;  // Store the average value of the sensor feedback
int buf[10], temp;

//time variables
int hours = 0;
int minutes = 0;
int seconds = 0;

// Function declarations:
int pollSensorsReservoir_1();
int pollSensorsReservoir_0();
float pollSensorspH();
void relayControl();
void connectToWiFi();
void sendDataToServer(int reservoir1Percentage, float reservoir2Fill, float phValue);
void sendPumpDurationToServer(String startDateTime, unsigned long duration);
String getFormattedDateTime();

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor_Empty_reservoir_1, INPUT);
  pinMode(sensor_Low_reservoir_1, INPUT);
  pinMode(sensor_Mid_reservoir_1, INPUT);
  pinMode(sensor_High_reservoir_1, INPUT);
  pinMode(sensor_Low_reservoir_0, INPUT);
  pinMode(sensor_Mid_reservoir_0, INPUT);
  pinMode(sensor_High_reservoir_0, INPUT);
  pinMode(pH_sensor, INPUT);
  pinMode(relay, OUTPUT);

  connectToWiFi();

  // Initialize NTP client
  timeClient.begin();
  timeClient.setTimeOffset(3600); // Timezone offset for Brussels (UTC+1)
}

void loop() {
  relayControl();
  delay(1000); // Delay to avoid spamming requests, adjust as needed
  static int lastReservoir1Percentage = 0;
  static int lastReservoir0Percentage = 0;
  static float lastPhValue = -1.0;

  int currentReservoir1Percentage = pollSensorsReservoir_1();
  //int currentReservoir0Percentage = pollSensorsReservoir_0();
  float currentPhValue = pollSensorspH();

  if (currentReservoir1Percentage != lastReservoir1Percentage ) {
    //*currentReservoir0Percentage != lastReservoir0Percentage || 
    //currentPhValue != lastPhValue
    
    sendDataToServer(0, currentReservoir1Percentage, 10);
    
    lastReservoir1Percentage = currentReservoir1Percentage;
    //lastReservoir0Percentage = currentReservoir0Percentage;
   // lastPhValue = currentPhValue;
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

// Function to poll sensors for Reservoir 1
int pollSensorsReservoir_1() {
  int sensor_Empty_reservoir_1_value = digitalRead(sensor_Empty_reservoir_1);
  int sensor_Low_reservoir_1_value = digitalRead(sensor_Low_reservoir_1);
  int sensor_Mid_reservoir_1_value = digitalRead(sensor_Mid_reservoir_1);
  int sensor_High_reservoir_1_value = digitalRead(sensor_High_reservoir_1);

  Serial.print("Reservoir 1: ");
  if(sensor_High_reservoir_1_value == 1)
  {
    Serial.println("210l left");
    return 95;
  }
  else if(sensor_Mid_reservoir_1_value == 1)
  {
    Serial.println("135l left");
    return 75;
  }
  else if(sensor_Low_reservoir_1_value == 1)
  {
    Serial.println("70l left");
    return 30;
  }
  else if(sensor_Empty_reservoir_1_value == 1)
  {
    Serial.println("25l left");
    return 10;
  }
  else
  {
    Serial.println("<25l left");
  }
  return 0; // Empty reservoir
}

// Function to poll sensors for Reservoir 0
int pollSensorsReservoir_0() {
  int sensor_Low_reservoir_0_value = digitalRead(sensor_Low_reservoir_0);
  int sensor_Mid_reservoir_0_value = digitalRead(sensor_Mid_reservoir_0);
  int sensor_High_reservoir_0_value = digitalRead(sensor_High_reservoir_0);

  if(sensor_High_reservoir_0_value == 1) {
    return 100; // Reservoir 0 is full
  } else if(sensor_Mid_reservoir_0_value == 1) {
    return 50;
  } else if(sensor_Low_reservoir_0_value == 1) {
    return 0;
  }
}

// Function to poll pH sensor
float pollSensorspH() {
  for(int i = 0; i < 10; i++) {
    buf[i] = analogRead(pH_sensor);
    delay(10);
  }

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
  for(int i = 2; i < 8; i++) avgValue += buf[i];
  
  float phValue = (float)avgValue * 5.0 / 1024 / 6;
  phValue = -5.70 * phValue + calibration_value; // Convert to pH

  return phValue;
}

// Function to control relay (pump)
void relayControl() {
  int sensor_High_reservoir_0_value = digitalRead(sensor_High_reservoir_0);
  int sensor_Low_reservoir_0_value = digitalRead(sensor_Low_reservoir_0);
  int sensor_High_reservoir_1_value = digitalRead(sensor_High_reservoir_1);
  int sensor_Empty_reservoir_1_value = digitalRead(sensor_Empty_reservoir_1);

  static bool pumpOn = false;
  static unsigned long pumpStartTime = 0; // Store the start time in milliseconds
  static String pumpStartDateTime = ""; // Store the full start datetime as a string for JSON

  // Update the NTP client for accurate time
  timeClient.update();

  if (pumpOn == false) {
    // Check conditions to turn the pump ON
    if ((sensor_High_reservoir_0_value == HIGH && sensor_High_reservoir_1_value == LOW) || 
        (sensor_Empty_reservoir_1_value == LOW && sensor_Low_reservoir_0_value == HIGH)) {
      digitalWrite(relay, HIGH);
      pumpOn = true;
      pumpStartTime = millis(); // Record the start time

      // Construct the ISO 8601 start time with date
      pumpStartDateTime = getFormattedDateTime();
      Serial.println("Pump ON");
    }
  } else {
    // Check conditions to turn the pump OFF
    if (sensor_Low_reservoir_0_value == LOW || sensor_High_reservoir_1_value == HIGH) {
      digitalWrite(relay, LOW);
      pumpOn = false;
      Serial.println("Pump OFF");

      // Calculate duration in seconds
      unsigned long pumpDuration = (millis() - pumpStartTime) / 1000;
      sendPumpDurationToServer(pumpStartDateTime, pumpDuration);
    }
  }
}

// Function to send pump duration data to the server
void sendPumpDurationToServer(String startDateTime, unsigned long duration) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Prepare the server endpoint
    String url = serverURL + "/api/pump-duration"; // Adjust the endpoint to your server's API
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    // Create the JSON payload
    String payload = "{";
    payload += "\"start_time\": \"" + startDateTime + "\",";
    payload += "\"duration\": " + String(duration);
    payload += "}";

    // Print the payload for debugging
    Serial.println("Sending pump duration data:");
    Serial.println(payload);

    // Send the POST request
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.println("Pump duration data sent successfully.");
    } else {
      Serial.print("Error sending data: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected, unable to send pump duration data.");
  }
}

// Function to get the full date and time in ISO 8601 format
String getFormattedDateTime() {
  // Get current time from NTP
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *timeInfo = gmtime((time_t*)&epochTime);

  char dateTimeBuffer[25];
  snprintf(dateTimeBuffer, sizeof(dateTimeBuffer), "%04d-%02d-%02dT%02d:%02d:%02dZ",
           timeInfo->tm_year + 1900,  // Year
           timeInfo->tm_mon + 1,     // Month (0-11 -> 1-12)
           timeInfo->tm_mday,        // Day
           timeInfo->tm_hour,        // Hour
           timeInfo->tm_min,         // Minute
           timeInfo->tm_sec);        // Second

  return String(dateTimeBuffer);
}


// Function to send data to the server
void sendDataToServer(int reservoir1Percentage, float reservoir2Fill, float phValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Send Reservoir 1 data
    String url = serverURL + "/api/reservoir1-fill"; // Change this to the appropriate endpoint
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"reservoir1_fill\": " + String(reservoir1Percentage);
    payload += "}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.println("Reservoir 1 data sent successfully.");
    } else {
      Serial.print("Error sending data: ");
      Serial.println(httpResponseCode);
    }
    http.end();

    // Send Reservoir 2 and pH data
    url = serverURL + "/api/reservoir2-fill"; // Change this to the appropriate endpoint
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    payload = "{";
    payload += "\"reservoir2_fill\": " + String(reservoir2Fill);
    payload += ", \"ph_level\": " + String(phValue);
    payload += ", \"nutrient_concentration\": " + String(420); // Example nutrient concentration
    payload += "}";

    Serial.println(payload);
    

    httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.println(httpResponseCode);
      Serial.println("Reservoir 2 and pH data sent successfully.");
    } else {
      Serial.print("Error sending data: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected, unable to send data");
  }
}
