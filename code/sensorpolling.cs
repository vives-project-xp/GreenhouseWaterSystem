using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vlotter_program_v1
{
    internal class Class1
    {
        //This is a class to read out multiple vlotter sensors to determine the value of a water reservoir

        //variables
        private int trigger_Ph_sensor = 0;

        private int sensor_Low_reservoir1 = 1; //sensor for 10l left
        private int sensor_Mid_reservoir1 = 2; //sensor for 25l left
        private int sensor_High_reservoir1 = 3; //sensor for 40l left

        private int sensor_Empty_reservoir2 = 4; //sensor for 20l left
        private int sensor_Low_reservoir2 = 5; //sensor for 50l left
        private int sensor_Mid_reservoir2 = 6; //sensor for 150l left
        private int sensor_High_reservoir2 = 7; //sensor for 200l left

        //analog sensor for Ph value of water
        private int sensor_Ph = A1;
        private int sensor_EC = A2;

        //poll the sensors
        private void pollSensors()
        {
            trigger_Ph_sensor = 1;
            //read the sensors
            int Ph_value = analogRead(sensor_Ph);
            int EC_value = analogRead(sensor_EC);
            //send the values to the user
            Serial.println("Ph value: " + Ph_value);
            Serial.println("EC value: " + EC_value);
        }
        //poll sensors for reservoir 1
        private void pollSensorsReservoir1()
        {
            //read the sensors
            int Low_reservoir1_value = digitalRead(sensor_Low_reservoir1);
            int Mid_reservoir1_value = digitalRead(sensor_Mid_reservoir1);
            int High_reservoir1_value = digitalRead(sensor_High_reservoir1);
            
            int Empty_reservoir2_value = digitalRead(sensor_Empty_reservoir2);
            int Low_reservoir2_value = digitalRead(sensor_Low_reservoir2);
            int Mid_reservoir2_value = digitalRead(sensor_Mid_reservoir2);
            int High_reservoir2_value = digitalRead(sensor_High_reservoir2);



            if (Low_reservoir1_value == 0)
            {
                //send a message to the user that the reservoir is almost empty
                Serial.println("Reservoir 1 is almost empty (10l left)");
            }
            else if (Mid_reservoir1_value == 0)
            {
                //send a message to the user that the reservoir is half full
                Serial.println("Reservoir 1 is half full (25l left)");
            }
            else if (High_reservoir1_value == 0)
            {
                //send a message to the user that the reservoir is full
                Serial.println("Reservoir 1 is full (40l left)");
            }

            if (Empty_reservoir2_value == 0)
            {
                //send a message to the user that the reservoir is empty
                Serial.println("Reservoir 2 is almost empty (20l left)");
            }
            else if (Low_reservoir2_value == 0)
            {
                //send a message to the user that the reservoir is almost empty
                Serial.println("Reservoir 2 is almost empty (50l left)");
            }
            else if (Mid_reservoir2_value == 0)
            {
                //send a message to the user that the reservoir is half full
                Serial.println("Reservoir 2 is half full (150l left)");
            }
            else if (High_reservoir2_value == 0)
            {
                //send a message to the user that the reservoir is full
                Serial.println("Reservoir 2 is full (200l left)");
            }
        }

    }
}
