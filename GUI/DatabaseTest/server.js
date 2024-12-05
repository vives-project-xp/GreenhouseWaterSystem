const express = require('express');
const mysql = require('mysql2');
const cors = require('cors');
const { exec } = require('child_process');


const app = express();
const port = 3000;

// Middleware
app.use(cors());
app.use(express.json());

// MySQL connection
const db = mysql.createConnection({
    host: '127.0.0.1',
    user: 'water_master', // Update this to your MySQL user
    password: 'GHWS4', // Add your MySQL password here
    database: 'water_collection_system'
});

// Connect to the database
db.connect((err) => {
    if (err) throw err;
    console.log('MySQL Connected...');
});

// API endpoint to get reservoir data
app.get('/api/reservoirs', (req, res) => {
    const query = `
        SELECT  
            (SELECT water_level FROM reservoir_1 ORDER BY timestamp DESC LIMIT 1) AS water_level_1,
            (SELECT water_level FROM reservoir_2 ORDER BY timestamp DESC LIMIT 1) AS water_level_2,
            (SELECT ph_level FROM reservoir_2 ORDER BY timestamp DESC LIMIT 1) AS ph_level,
            (SELECT nutrient_concentration FROM reservoir_2 ORDER BY timestamp DESC LIMIT 1) AS nutrient_concentration
    `;

    db.query(query, (err, results) => {
        if (err) {
            console.error('Error executing query:', err);
            return res.status(500).send('Internal Server Error');
        }
        res.json(results);
    });
});

// API endpoint to get reservoir and pump logs
app.get('/api/logs', (req, res) => {
    const reservoir1Query = `SELECT * FROM reservoir_1 ORDER BY timestamp DESC`;
    const reservoir2Query = `SELECT * FROM reservoir_2 ORDER BY timestamp DESC`;
    const pumpQuery = `SELECT * FROM pump_activity ORDER BY timestamp_on DESC`;

    db.query(reservoir1Query, (err, reservoir1Results) => {
        if (err) throw err;

        db.query(reservoir2Query, (err, reservoir2Results) => {
            if (err) throw err;

            db.query(pumpQuery, (err, pumpResults) => {
                if (err) throw err;

                res.json({
                    reservoir1: reservoir1Results,
                    reservoir2: reservoir2Results,
                    pump: pumpResults
                });
            });
        });
    });
});

// API endpoint to get the most recent rain prediction data for 1, 3, and 7 days
app.get('/api/rain', (req, res) => {
    const query = `
        SELECT days_ahead, precipitation, created_at
        FROM rain_prediction
        WHERE created_at = (
            SELECT MAX(created_at)
            FROM rain_prediction AS r
            WHERE r.days_ahead = rain_prediction.days_ahead
        )
        ORDER BY days_ahead ASC
    `;

    db.query(query, (err, results) => {
        if (err) {
            console.error('Error executing query:', err);
            return res.status(500).send('Internal Server Error');
        }
        res.json(results);
    });
});

// New endpoint to save roof collection data with dynamic days_ahead
app.post('/api/roof-collection', (req, res) => {
    const { total_roof_collection_liters, days_ahead } = req.body;

    if (total_roof_collection_liters === undefined || days_ahead === undefined) {
        return res.status(400).send('Both total_roof_collection_liters and days_ahead are required');
    }

    const query = `
        INSERT INTO rain_prediction (days_ahead, precipitation, created_at)
        VALUES (?, ?, NOW())
    `;

    db.query(query, [days_ahead, total_roof_collection_liters], (err, result) => {
        if (err) {
            console.error('Error inserting roof collection data:', err);
            return res.status(500).send('Internal Server Error');
        }
        res.status(200).send('Roof collection data saved successfully');
    });
});



// POST request to trigger Python script
app.post('/run-python', (req, res) => {
    // Run the Python script here
    exec('C:/Users/Thomas/miniconda3/python.exe ./WheatherForcast/HemelWaterToevoer.py', (err, stdout, stderr) => {
        if (err) {
            console.error(`Error: ${stderr}`);
            return res.status(500).json({ message: 'Error executing Python script.' });
        }

        console.log(`Python script output: ${stdout}`);
        res.status(200).json({ message: 'Python script executed successfully.' });
    });
});

app.get('/api/pump-status', (req, res) => {
    const query = `
        SELECT timestamp_on AS lastOn, timestamp_off AS lastOff, duration
        FROM pump_activity
        ORDER BY timestamp_on DESC
        LIMIT 1
    `;

    db.query(query, (err, results) => {
        if (err) {
            console.error('Error fetching pump status:', err);
            return res.status(500).send('Internal Server Error');
        }

        if (results.length > 0) {
            const rawData = results[0];

            // Format the date
            const lastOnDate = new Date(rawData.lastOn);
            const formattedLastOn = `${String(lastOnDate.getDate()).padStart(2, '0')}/${String(lastOnDate.getMonth() + 1).padStart(2, '0')}/${lastOnDate.getFullYear()} ${String(lastOnDate.getHours()).padStart(2, '0')}:${String(lastOnDate.getMinutes()).padStart(2, '0')}:${String(lastOnDate.getSeconds()).padStart(2, '0')}`;

            // Log and respond with formatted data
            const response = {
                lastOn: formattedLastOn,
                duration: rawData.duration
            };

            //console.log('Pump Status Data:', response); // Log the formatted data
            res.json(response);
        } else {
            console.log('No pump data available'); // Log if no data found
            res.json(null);
        }
    });
});


app.post('/api/pump-duration', (req, res) => {
    const { start_time, duration } = req.body;

    // Validate inputs
    if (!start_time || !duration || isNaN(duration) || duration <= 0) {
        return res.status(400).send('Start time and a positive duration are required.');
    }

    // Calculate stop time
    const stopTime = new Date(new Date(start_time).getTime() + duration * 1000); // Convert duration to milliseconds

    // Insert data into the database
    const query = `
        INSERT INTO pump_activity (timestamp_on, timestamp_off, duration) 
        VALUES (?, ?, ?)
    `;

    db.query(query, [new Date(start_time), stopTime, duration], (err, result) => {
        if (err) {
            console.error('Error inserting pump activity:', err);
            return res.status(500).send('Failed to log pump activity.');
        }

        console.log(`Pump activity logged: Start Time = ${start_time}, Stop Time = ${stopTime}, Duration = ${duration}s`);
        res.status(200).send('Pump activity logged successfully.');
    });
});


// Endpoint to update Reservoir 1 fill percentage
app.post('/api/reservoir1-fill', (req, res) => {
    const { reservoir1_fill } = req.body;

    if (reservoir1_fill === undefined) {
        return res.status(400).send('Reservoir 1 fill percentage is required');
    }

    const query = `
        INSERT INTO reservoir_1 (water_level, timestamp)
        VALUES (?, NOW())
    `;

    db.query(query, [reservoir1_fill], (err, result) => {
        if (err) {
            console.error('Error updating Reservoir 1 fill percentage:', err);
            return res.status(500).send('Internal Server Error');
        }
        res.status(200).send('Reservoir 1 fill percentage updated successfully');
    });
});

// Endpoint to update Reservoir 2 fill percentage, pH level, and nutrient concentration
app.post('/api/reservoir2-fill', (req, res) => {
    const { reservoir2_fill, ph_level, nutrient_concentration } = req.body;

    if (reservoir2_fill === undefined || ph_level === undefined || nutrient_concentration === undefined) {
        return res.status(400).send('Reservoir 2 fill percentage, pH level, and nutrient concentration are required');
    }

    const query = `
        INSERT INTO reservoir_2 (water_level, ph_level, nutrient_concentration, timestamp)
        VALUES (?, ?, ?, NOW())
    `;

    db.query(query, [reservoir2_fill, ph_level, nutrient_concentration], (err, result) => {
        if (err) {
            console.error('Error updating Reservoir 2 data:', err);
            return res.status(500).send('Internal Server Error');
        }
        res.status(200).send('Reservoir 2 data updated successfully');
    });
});



// Root route to respond at http://localhost:3000/
app.get('/', (req, res) => {
    res.send('Welcome to the Water Collection System API');
});


// Start the server
app.listen(port, '0.0.0.0', () => {
    console.log(`Server running at http://0.0.0.0:${port}`);
});
