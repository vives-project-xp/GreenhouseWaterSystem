const express = require('express');
const mysql = require('mysql2');
const cors = require('cors');

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
        console.log('Query Results:', results);
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

// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
