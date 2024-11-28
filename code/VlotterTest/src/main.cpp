#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define sensor_Empty_reservoir_1 0 //sensor for 35 left
#define sensor_Low_reservoir_1  1 //sensor for 80 left
#define sensor_Mid_reservoir_1  2 //sensor for 165 left
#define sensor_High_reservoir_1 3 //sensor for 210 left
#define pH_sensor 4 //sensor for pH
#define relay 5 //relay for pump
#define sensor_Low_reservoir_0 6 //sensor for 25 left
#define sensor_Mid_reservoir_0  7 //sensor for 70 left
#define sensor_High_reservoir_0  8 //sensor for 185 left

const char* ssid = "devbit";
const char* password = "Dr@@dloos!";
const String serverURL = "http://your_server_ip:3000"; // Replace with your server's IP

float calibration_value = 21.34 + 0.7;
unsigned long int avgValue;  // Store the average value of the sensor feedback
int buf[10], temp;

// Function declarations:
void pollSensorsReservoir_1();
void pollSensorsReservoir_0();
void pollSensorspH();
void relayControl();
void connectToWiFi();
void sendDataToServer(int reservoir1Percentage, float reservoir2Fill, float phValue);

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
}

void loop() {
  relayControl();
  pollSensorsReservoir_1();
  pollSensorsReservoir_0();
<<<<<<< HEAD
  delay(1000);
=======
  pollSensorspH();
>>>>>>> 1329779d1514f02bb7e88924ed188c161d9aa727
  
  // After polling, send data to the server
  sendDataToServer(50, 60, 6.5);
  
  delay(5000); // Delay to avoid spamming requests, adjust as needed
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

<<<<<<< HEAD
  Serial.print("Reservoir 1: ");
  if(sensor_High_reservoir_1_value == 1)
  {
    Serial.println("210l left");
  }
  else if(sensor_Mid_reservoir_1_value == 1)
  {
    Serial.println("135l left");
  }
  else if(sensor_Low_reservoir_1_value == 1)
  {
    Serial.println("70l left");
  }
  else if(sensor_Empty_reservoir_1_value == 1)
  {
    Serial.println("25l left");
  }
  else
  {
    Serial.println("<25l left");
=======
  if(sensor_High_reservoir_1_value == 1) {
    return 210; // Reservoir 1 is full
  } else if(sensor_Mid_reservoir_1_value == 1) {
    return 135;
  } else if(sensor_Low_reservoir_1_value == 1) {
    return 70;
  } else if(sensor_Empty_reservoir_1_value == 1) {
    return 25;
>>>>>>> 1329779d1514f02bb7e88924ed188c161d9aa727
  }
  return 0; // Empty reservoir
}

// Function to poll sensors for Reservoir 0
int pollSensorsReservoir_0() {
  int sensor_Low_reservoir_0_value = digitalRead(sensor_Low_reservoir_0);
  int sensor_Mid_reservoir_0_value = digitalRead(sensor_Mid_reservoir_0);
  int sensor_High_reservoir_0_value = digitalRead(sensor_High_reservoir_0);

  if(sensor_High_reservoir_0_value == 1) {
    return 70; // Reservoir 0 is full
  } else if(sensor_Mid_reservoir_0_value == 1) {
    return 50;
  } else if(sensor_Low_reservoir_0_value == 1) {
    return 25;
  }
  return 0; // Empty reservoir
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

<<<<<<< HEAD
  if (pumpOn == false) {
    // Check conditions to turn the pump ON
    if ((sensor_High_reservoir_0_value == HIGH && sensor_High_reservoir_1_value == LOW) || (sensor_Empty_reservoir_1_value == LOW && sensor_Low_reservoir_0_value == HIGH)) {
=======
  if (!pumpOn) {
    if (sensor_High_reservoir_0_value == HIGH || sensor_Empty_reservoir_1_value == LOW) {
>>>>>>> 1329779d1514f02bb7e88924ed188c161d9aa727
      digitalWrite(relay, HIGH);
      pumpOn = true;
      Serial.println("Pump ON");
    }
  } else {
    if (sensor_Low_reservoir_0_value == LOW || sensor_High_reservoir_1_value == HIGH) {
      digitalWrite(relay, LOW);
      pumpOn = false;
      Serial.println("Pump OFF");
    }
  }
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

    // Send Reservoir 2 and pH data
    url = serverURL + "/api/reservoir2-fill"; // Change this to the appropriate endpoint
    http.begin(url);
    payload = "{";
    payload += "\"reservoir2_fill\": " + String(reservoir2Fill);
    payload += ", \"ph_level\": " + String(phValue);
    payload += ", \"nutrient_concentration\": " + String(420); // Example nutrient concentration
    payload += "}";

    httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
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
