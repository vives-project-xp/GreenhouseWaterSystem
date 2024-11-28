#include <Arduino.h>


#define sensor_Empty_reservoir 0 //sensor for 20l left
#define sensor_Low_reservoir  1 //sensor for 50l left
#define sensor_Mid_reservoir  2 //sensor for 150l left
#define sensor_High_reservoir 3 //sensor for 200l left
#define pH_sensor 4 //sensor for pH
#define relay 5 //relay for pump
float calibration_value = 21.34 + 0.7;
unsigned long int avgValue;  //Store the average value of the sensor feedback
int buf[10], temp;

// put function declarations here:
void pollSensorsReservoir();
void pollSensorspH();
void relayControl();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor_Empty_reservoir, INPUT);
  pinMode(sensor_Low_reservoir, INPUT);
  pinMode(sensor_Mid_reservoir, INPUT);
  pinMode(sensor_High_reservoir, INPUT);
  pinMode(pH_sensor, INPUT);
  pinMode(relay, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  relayControl();
  
}

// put function definitions here:
void pollSensorsReservoir()
{
  int sensor_Empty_reservoir_value = digitalRead(sensor_Empty_reservoir);
  int sensor_Low_reservoir_value = digitalRead(sensor_Low_reservoir);
  int sensor_Mid_reservoir_value = digitalRead(sensor_Mid_reservoir);
  int sensor_High_reservoir_value = digitalRead(sensor_High_reservoir);

  if(sensor_High_reservoir_value == 1)
  {
    Serial.println("210l left");
  }
  else if(sensor_Mid_reservoir_value == 1)
  {
    Serial.println("135l left");
  }
  else if(sensor_Low_reservoir_value == 1)
  {
    Serial.println("70l left");
  }
  else if(sensor_Empty_reservoir_value == 1)
  {
    Serial.println("25l left");
  }
  else
  {
    Serial.println("0l left");
  }
}

void pollSensorspH()
{
  for(int i=0;i<10;i++){ //Get 10 sample value from the sensor for smooth the value
    buf[i]=analogRead(pH_sensor);
    delay(10);
  }

  Serial.print("value: ");
  Serial.println(analogRead(pH_sensor));

  for(int i=0;i<9;i++){ //sort the analog from small to large
    for(int j=i+1;j<10;j++){
      if(buf[i]>buf[j]){
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)avgValue+=buf[i];  //take the average value of 6 center sample
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue = -5.70 * phValue + calibration_value; //convert the millivolt into pH value

  if(phValue < 4) Serial.println("very acidic");
  else if(phValue>=4  && phValue<5) Serial.println("acidic");
  else if(phValue>=5  && phValue<7) Serial.println("Acidic-ish");
  else if(phValue>=7  && phValue<8) Serial.println("neutral");
  else if(phValue>=8  && phValue<10) Serial.println("Alkaline-ish");
  else if(phValue>=10 && phValue<11) Serial.println("alkaline");
  else if(phValue>=11) Serial.println("very alkaline");
}

void relayControl()
{
  digitalWrite(relay, HIGH);
  delay(5000);
  digitalWrite(relay, LOW);
  delay(5000);
}
