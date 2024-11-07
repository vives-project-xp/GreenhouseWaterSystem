import pandas as pd
import requests
from datetime import datetime
import Forcast

# Constants
SURFACE_AREA = 6  # in square meters

# Load precipitation data
data = pd.read_csv('./hourly_precipitation_data.csv')

# Get the current hour with winter time adjustment
current_hour = datetime.now().hour + 2

# Skip the rows up to the current hour to exclude past data
data_filtered = data.iloc[current_hour:]

# Define the hours representing 1, 3, and 7 days
hours_1_day = 24
hours_3_days = 72
hours_7_days = 168

# Calculate total precipitation for each period in mm
total_precipitation_1_day = data_filtered['rain'].iloc[:hours_1_day].sum()
total_precipitation_3_days = data_filtered['rain'].iloc[:hours_3_days].sum()
total_precipitation_7_days = data_filtered['rain'].iloc[:hours_7_days].sum()

# Calculate roof collection in liters for each period
roof_collection_1_day = total_precipitation_1_day * SURFACE_AREA
roof_collection_3_days = total_precipitation_3_days * SURFACE_AREA
roof_collection_7_days = total_precipitation_7_days * SURFACE_AREA

# Display results
print(f"Total precipitation for the next 1 day: {total_precipitation_1_day} mm")
print(f"Total roof collection for 1 day: {roof_collection_1_day} liters")

print(f"Total precipitation for the next 3 days: {total_precipitation_3_days} mm")
print(f"Total roof collection for 3 days: {roof_collection_3_days} liters")

print(f"Total precipitation for the next 7 days: {total_precipitation_7_days} mm")
print(f"Total roof collection for 7 days: {roof_collection_7_days} liters")

# Send data to the Express server for each forecast
url = 'http://localhost:3000/api/roof-collection'

# Define and send payloads for each forecast period
forecasts = [
    {'total_roof_collection_liters': roof_collection_1_day, 'days_ahead': 1},
    {'total_roof_collection_liters': roof_collection_3_days, 'days_ahead': 3},
    {'total_roof_collection_liters': roof_collection_7_days, 'days_ahead': 7}
]

for forecast in forecasts:
    try:
        response = requests.post(url, json=forecast)
        if response.status_code == 200:
            print(f"Roof collection data for {forecast['days_ahead']} days sent successfully.")
        else:
            print(f"Failed to send data for {forecast['days_ahead']} days. Status code:", response.status_code)
            print("Response:", response.text)
    except requests.exceptions.RequestException as e:
        print(f"Error sending data for {forecast['days_ahead']} days:", e)
