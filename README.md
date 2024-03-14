# Electro-Pneumatic Shock Optimization System

This code is designed to control an electro-pneumatic shock optimization system using an Arduino microcontroller. The system consists of pressure sensors, solenoid valves, switches, and buttons to adjust and maintain desired pressure levels for shock optimization.

## Components

### Pressure Sensors
- Four pressure sensors (H1-H4) are connected to analog pins to measure pressure levels.
- Each pressure sensor is mapped to a range of 0-300 units.

### Solenoid Valves
- For each harness (H1-H4), there are two solenoid valves:
  - One for filling (F)
  - One for exhausting (E)
- Solenoid valves are controlled via digital pins to adjust pressure levels.

### Power Pins
- Digital pins are used to power the system components, ensuring proper operation.

### Switches and Buttons
- Switches:
  - Switch 1: Settings 1-4
  - Switch 2: Settings 5-8
- Buttons:
  - Button 1: Adjust pressure setting 1
  - Button 2: Adjust pressure setting 2
  - Button 3: Adjust pressure setting 3
  - Button 4: Adjust pressure setting 4

## Operation

The Arduino code reads analog values from pressure sensors, switch states, and button states. Based on this input, it updates the desired pressure level and adjusts the solenoid valves to maintain the desired pressure. The system is designed to optimize shock performance while ensuring safety by monitoring pressure levels.

## Communication with Python Script

This code communicates with a Python script to display pressure readings using a JSON file. The Arduino code serializes pressure readings, desired pressure, switch states, and button states into a JSON format. The Python script reads this JSON file and displays the data in a user-friendly format, providing real-time monitoring of pressure levels.

## Usage

1. Upload the provided Arduino code to your Arduino microcontroller.
2. Connect the pressure sensors, solenoid valves, switches, and buttons according to the pin configurations specified in the code.
3. Power up the system.
4. Run the Python script for real-time monitoring of pressure levels.
5. Use the switches and buttons to adjust the desired pressure settings as needed.
6. The system will automatically adjust the solenoid valves to maintain the desired pressure levels for shock optimization.
