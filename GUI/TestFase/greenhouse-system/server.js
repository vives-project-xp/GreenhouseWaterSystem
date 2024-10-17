const express = require('express');
const mysql = require('mysql');
const cors = require('cors');

// Maak een nieuwe Express-app aan
const app = express();
app.use(cors());

// Database configuratie
const db = mysql.createConnection({
    host: 'localhost',
    user: 'water_master', // verander naar jouw MySQL-gebruiker
    password: 'GHWS4', // voeg je MySQL-wachtwoord hier toe
    database: 'GHWS_Watermaster'
});

// Verbind met de MySQL-database
db.connect((err) => {
    if (err) {
        throw err;
    }
    console.log('Verbonden met de MySQL-database');
});

// Endpoint om sensorgegevens op te halen
app.get('/api/sensors', (req, res) => {
    let sql = 'SELECT sensor_type, sensor_value, timestamp FROM sensors ORDER BY timestamp DESC';
    db.query(sql, (err, results) => {
        if (err) {
            return res.status(500).send('Database error: ' + err);
        }
        res.json(results);
    });
});

// Server draaien op poort 3000
const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server draait op http://localhost:${PORT}`);
});
