create DATABASE	water_collection_system;

CREATE USER 'water_master'@'localhost' IDENTIFIED BY 'GHWS4';
GRANT ALL PRIVILEGES ON water_collection_system.* TO 'water_master'@'localhost';
FLUSH PRIVILEGES;

-- Table for the first reservoir (water level only)
CREATE TABLE IF NOT EXISTS reservoir_1 (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    water_level FLOAT NOT NULL -- Percentage of water level
);

-- Table for the second reservoir (water level, pH, and nutrient concentration)
CREATE TABLE IF NOT EXISTS reservoir_2 (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    water_level FLOAT NOT NULL,                 -- Percentage of water level
    ph_level FLOAT NOT NULL,                    -- pH level
    nutrient_concentration FLOAT NOT NULL       -- Nutrient concentration in ppm
);

-- Table for pump activity logs
CREATE TABLE IF NOT EXISTS pump_activity (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp_on DATETIME NOT NULL,
    timestamp_off DATETIME NOT NULL,
    duration INT NOT NULL -- Duration in seconds
);

-- Sample data (optional)
INSERT INTO reservoir_1 (water_level) VALUES
(50.0), -- Sample data for water level
(60.0); -- Another sample

INSERT INTO reservoir_2 (water_level, ph_level, nutrient_concentration) VALUES
(70.0, 7.2, 500.0), -- Sample data for reservoir 2
(80.0, 6.5, 300.0); -- Another sample

-- Sample pump activity (optional)
INSERT INTO pump_activity (timestamp_on, timestamp_off, duration) VALUES
(NOW(), NOW() + INTERVAL 10 SECOND, 10), -- Pump activated for 10 seconds
(NOW() + INTERVAL 20 MINUTE, NOW() + INTERVAL 30 MINUTE, 600); -- Pump activated for 10 minutes