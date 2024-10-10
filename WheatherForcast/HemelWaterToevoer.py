import pandas as pd
import Forcast 
from datetime import datetime

# chack what houre it is now
# only the hour is important, not the minute or second nor the day
now = datetime.now()
current_time = now.strftime("%H")
print("Current Time =", current_time)

# Constants
SURFACE_AREA = 18  # in square meters (example surface area)
RAIN_CONVERSION = 0.001  # Convert mm to meters for volume calculation


# Load the precipitation data
data = pd.read_csv('hourly_precipitation_data.csv')

# Calculate total precipitation over the next 3 days
# Filter for the next 3 days (72 hours)
# The current hour is not included in the calculation
# not count the firt current_time hours
total_precipitation_mm = data['precipitation'].sum()  # Sum of precipitation in mm

# Convert to meters for volume calculation
total_precipitation_m = total_precipitation_mm * RAIN_CONVERSION

# Calculate the volume of rain in cubic meters
volume_of_rain_cubic_m = total_precipitation_m * SURFACE_AREA

# Display results
print(f"Total precipitation over the next 3 days: {total_precipitation_mm} mm")
print(f"Volume of rain on {SURFACE_AREA} m² surface: {volume_of_rain_cubic_m} m³ = {volume_of_rain_cubic_m * 1000} liters")
