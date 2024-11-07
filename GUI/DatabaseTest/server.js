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

// Root route to respond at http://localhost:3000/
app.get('/', (req, res) => {
    res.send('Welcome to the Water Collection System API');
});


// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
