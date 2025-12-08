# flood-detection-system
Based on water level readings the system turn on and alarm and a water pump to evacuate the water from a tank.

Overview
This project was created to help reduce flooding in problem areas. The version shown here is a prototype, so there is still plenty of room for future improvements. The system monitors water levels inside a tank using an ultrasonic sensor. When the water reaches an unsafe threshold, a buzzer and LED turn on to warn of the danger. At the same time, a water pump activates to evacuate water from the tank until the level returns to a safe point. Once that happens, the alarm and pump turn off. The system continues monitoring water levels at all times, even while active. All readings, including water level, tank fill percentage, and pump status, are sent to the Blynk IoT platform.

Features
-Continuous water level monitoring
-Ultrasonic distance measurement
-Automatic alarm system using LED and buzzer
-Automatic pump activation during unsafe water levels
-Pump turns off once the tank returns to a safe range
-Real time data sent to Blynk including water level, tank percentage and pump status
-Modular code design with clear functions for each part of the system
-Flowcharts included to show program logic

Components Used
-ESP32 WROOM
-HC-SR04 ultrasonic sensor
-Mini submersible 12V DC pump
-LED with 220 ohm resistor
-Buzzer with 100 ohm resistor
-External 12V Power Supply for the pump
-Jumper wires
-Breadboard

How the System Works
The ultrasonic sensor sends out a sound pulse and measures the time it takes to return. This value is used to calculated the distance from the sensor to the water surface. The code converts this distance into a water height and percentange of how full the tank is.

When the water reaches an unsafe threshold the system enters the alert state. The buzzer and LED start blinking and the pump turns on to remove water. While the pump is running the system continues to measure the water level. Once the levels falls back to a safe point, the system turns everything off and returns to normal monitoring.

All important values are sent to Blynk. The dashboard shows the water level, how full the tank is and whether the pump is on or off.

File structure
/flood-detection-system
  code/
    main_project_code.ino

  images/
    schematics
    flowcharts
    circuit_diagrams

  README.md

Blynk Integration
The ESP32 connects to WiFi and sends the following values to the Blynk platform:
-Water level in inches
-Tank percentage
-Pump ON or OFF

These values can be displayed through the Blynk widgets such as gauges, text lables or value displays. All Blynk credentials have been removed from the public version of this repository. Users must insert their own credentials before uploading the code.

Setup Instructions
1. Install the ESP32 board support package in the Arduino IDE
2. Install requiered libraries:
   -WiFi
   -BlynkSimpleEsp32
   -NewPing
3. Open the .ino file in the code folder
4. Enter your own Blynk template ID, authentication token, WiFi name and        WiFi password
6. Build the circuit following the schematic diagram included in the images     folder
7. Open the Blynk dashboard, create a template, add the datastreams to view     the readings and the pump status

Images
![System Schematic](images/Schematic_Flood_Detection_system.png)
![Example Blynk Dashboard](Blynk_Web_App.png)    
