using System;
using MySql.Data.MySqlClient;

namespace Vlotter_program_v1
{
    internal class ReservoirMonitor
    {
        // Sensoren voor reservoir 1
        private int sensor_Low_reservoir1 = 1;  // 10L
        private int sensor_Mid_reservoir1 = 2;  // 25L
        private int sensor_High_reservoir1 = 3; // 40L

        // Sensoren voor reservoir 2
        private int sensor_Empty_reservoir2 = 4;  // 20L
        private int sensor_Low_reservoir2 = 5;    // 50L
        private int sensor_Mid_reservoir2 = 6;    // 150L
        private int sensor_High_reservoir2 = 7;   // 200L
        private int sensor_Ph = 8;                // pH sensor
        private int sensor_EC = 9;                // EC sensor

        // Databaseconfiguratie
        private string connectionString = "Server=localhost;Database=water_collection_system;Uid=water_master;Pwd=GHWS4;";

        // Methode om waterniveau van reservoir 1 op te slaan
        public void SaveReservoir1Data(float waterLevelPercentage)
        {
            using (var connection = new MySqlConnection(connectionString))
            {
                connection.Open();
                string query = "INSERT INTO reservoir_1 (water_level) VALUES (@waterLevel)";
                using (var command = new MySqlCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@waterLevel", waterLevelPercentage);
                    command.ExecuteNonQuery();
                }
            }
        }

        // Methode om waterniveau en pH van reservoir 2 op te slaan
        public void SaveReservoir2Data(float waterLevelPercentage, float ph, float nutrientConcentration)
        {
            using (var connection = new MySqlConnection(connectionString))
            {
                connection.Open();
                string query = "INSERT INTO reservoir_2 (water_level, ph_level, nutrient_concentration) VALUES (@waterLevel, @ph, @nutrientConcentration)";
                using (var command = new MySqlCommand(query, connection))
                {
                    command.Parameters.AddWithValue("@waterLevel", waterLevelPercentage);
                    command.Parameters.AddWithValue("@ph", ph);
                    command.Parameters.AddWithValue("@nutrientConcentration", nutrientConcentration);
                    command.ExecuteNonQuery();
                }
            }
        }

        // Simulatie van het uitlezen van sensoren en opslaan in de database
        public void PollAndSaveData()
        {
            // Lees waterniveau van reservoir 1
            float waterLevelReservoir1 = ReadReservoir1();
            SaveReservoir1Data(waterLevelReservoir1);

            // Lees waterniveau en pH van reservoir 2
            float waterLevelReservoir2 = ReadReservoir2();
            float phLevel = ReadPhSensor();
            float nutrientConcentration = ReadECSensor();
            SaveReservoir2Data(waterLevelReservoir2, phLevel, nutrientConcentration);
        }

        // Simulatiemethoden voor het uitlezen van sensoren
        private float ReadReservoir1()
        {
            // Simuleer waterniveau als percentage
            return 70.0f; // Voorbeeldwaarde
        }

        private float ReadReservoir2()
        {
            // Simuleer waterniveau als percentage
            return 85.0f; // Voorbeeldwaarde
        }

        private float ReadPhSensor()
        {
            // Simuleer pH-waarde
            return 7.2f; // Voorbeeldwaarde
        }

        private float ReadECSensor()
        {
            // Simuleer EC-concentratie
            return 500.0f; // Voorbeeldwaarde
        }
    }
}
