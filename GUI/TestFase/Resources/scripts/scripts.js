// Hier kun je de grafieken gaan initialiseren met een bibliotheek zoals Chart.js
// Dit is een voorbeeld van hoe je de pH-grafiek zou kunnen implementeren

const ctx = document.getElementById('phChart').getContext('2d');
const phChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: ['6:00', '9:00', '12:00', '15:00', '18:00'],
        datasets: [{
            label: 'pH-Waarde',
            data: [6.3, 6.5, 6.4, 6.6, 6.5],
            borderColor: 'rgba(52, 152, 219, 1)',
            fill: false
        }]
    },
    options: {
        responsive: true
    }
});


function showSection(sectionId) {
    // Verberg alle secties
    const sections = document.querySelectorAll('.section');
    sections.forEach(section => section.classList.add('hidden'));

    // Toon de geselecteerde sectie
    document.getElementById(sectionId).classList.remove('hidden');
    
    // Verwijder 'active' klasse van alle links
    const navLinks = document.querySelectorAll('nav ul li a');
    navLinks.forEach(link => link.classList.remove('active'));

    // Voeg 'active' klasse toe aan de geklikte link
    const activeLink = document.querySelector(`nav ul li a[onclick="showSection('${sectionId}')"]`);
    activeLink.classList.add('active');
}

document.addEventListener('DOMContentLoaded', function() {
    // Functie om sensorgegevens op te halen
    function loadSensorData() {
        fetch('http://localhost:3000/api/sensors')
            .then(response => response.json())
            .then(data => {
                // Selecteer het element waar je de data wilt tonen (bijv. in het dashboard)
                const sensorTable = document.querySelector('#sensors tbody');
                sensorTable.innerHTML = ''; // Maak de huidige inhoud leeg

                // Loop door de data en vul de tabel
                data.forEach(sensor => {
                    const row = `<tr>
                                    <td>${sensor.sensor_type}</td>
                                    <td>${sensor.sensor_value}</td>
                                    <td>${sensor.timestamp}</td>
                                </tr>`;
                    sensorTable.innerHTML += row;
                });
            })
            .catch(error => console.error('Error fetching sensor data:', error));
    }

    // Haal direct de sensorgegevens op bij het laden van de pagina
    loadSensorData();
});
