EnviroGuard — System User Guide
1. System Overview

EnviroGuard is a smart environmental monitoring and security system that collects data from connected sensors and displays the information through a web-based dashboard.

The system monitors:

Temperature
Humidity
Gas/Smoke level
Motion
Alarm status
System health and connection status

The Arduino Uno collects the sensor readings and communicates them to the ESP32. The ESP32 sends the collected data to the backend API, where the data can be stored and accessed by the web dashboard.

2. System Requirements

Before using EnviroGuard, make sure the following components are available and properly connected:

Hardware
Arduino Uno
ESP32
DHT22 temperature and humidity sensor
PIR motion sensor
MQ-2 gas/smoke sensor
Buzzer
Logic level shifter
Breadboard and jumper wires
USB cables
Computer connected to the same network as the ESP32
Software
Visual Studio Code
PlatformIO
Node.js
EnviroGuard backend/API
Web browser
3. Starting the System
Step 1 — Connect the Hardware

Connect the Arduino Uno, ESP32, sensors, buzzer, and logic level shifter according to the system wiring diagram.

Make sure that:

The DHT22 is connected to the Arduino Uno.
The PIR sensor is connected to the Arduino Uno.
The MQ-2 sensor is connected to the Arduino Uno.
The buzzer is connected to the Arduino Uno.
The Arduino Uno and ESP32 communicate through the appropriate serial connection.
The logic level shifter is used where necessary between the 5V Arduino and 3.3V ESP32 signals.
All components share the required ground connections.
4. Start the Arduino Uno
Step 2 — Connect the Arduino Uno

Connect the Arduino Uno to the computer using a USB cable.

Open the EnviroGuard Arduino project in Visual Studio Code with PlatformIO.

Check that the correct board and serial port are selected.

Step 3 — Upload the Arduino Program

Upload the Arduino Uno program using PlatformIO.

The program initializes the sensors and continuously collects:

Temperature
Humidity
Gas/smoke readings
Motion status

The Arduino also evaluates the configured alarm conditions and controls the buzzer when an alarm condition is detected.

5. Start the ESP32
Step 4 — Connect the ESP32

Connect the ESP32 to the computer using its USB cable.

Open the ESP32 project in Visual Studio Code.

Make sure the ESP32 is connected to the same network that will be used by the backend server.

Step 5 — Upload the ESP32 Program

Upload the ESP32 program using PlatformIO.

The ESP32 receives sensor data from the Arduino Uno and prepares the information for communication with the EnviroGuard backend.

Open the Serial Monitor if necessary to verify that the ESP32 is receiving sensor data correctly.

6. Start the Backend API
Step 6 — Open the Backend Project

Open a terminal in the EnviroGuard backend project directory.

Install the required Node.js dependencies if this is the first time running the project.

Step 7 — Start the Server

Start the EnviroGuard backend server using the configured Node.js command.

Once the server starts successfully, the API will be available through the configured local address.

For example:

http://localhost:3000

The backend provides the API endpoints used by the dashboard and communicates with the database.

7. Check the System Health
Step 8 — Test the Health Endpoint

Open the following endpoint in a browser:

http://localhost:3000/health

The health endpoint checks whether the system services are operating correctly.

A successful response indicates that the backend and its required services are available.

If the endpoint returns:

503 Service Unavailable

this means that one or more required services are currently unavailable or not ready. Check that the backend server and required database/device connections are running.

8. Open the EnviroGuard Dashboard
Step 9 — Open the Web Dashboard

Open the EnviroGuard dashboard in a web browser.

The dashboard provides a centralized interface for viewing the environmental and security information collected by the system.

The dashboard displays the latest available sensor information and system status.

9. View Sensor Data
Step 10 — Monitor Temperature and Humidity

The temperature and humidity values collected by the DHT22 sensor are displayed on the dashboard.

The user can observe the current:

Temperature
Humidity
Last updated time

The values are periodically updated as new sensor readings are received.

Step 11 — Monitor Gas/Smoke Level

The MQ-2 sensor provides a gas/smoke reading.

The dashboard displays the current gas/smoke level so the user can monitor changes in the environment.

If the configured gas threshold is exceeded, the system can activate the alarm.

Step 12 — Monitor Motion

The PIR sensor detects movement within its detection range.

The dashboard displays the current motion status.

The motion status can indicate whether movement has been detected by the sensor.

If motion-based alarms are enabled, detecting motion can also trigger the system alarm.

10. Monitor the Alarm
Step 13 — Check Alarm Status

The dashboard displays the current alarm status.

The alarm can be triggered when an enabled sensor exceeds its configured threshold or detects a configured security condition.

Possible alarm sources include:

High temperature
High gas/smoke level
Detected motion

The buzzer provides a physical indication when an alarm condition is active.

11. Monitor System Information
Step 14 — Check the System Status

The dashboard also provides system information such as:

Last updated time
Current date
Time zone
Connection/system status
API/system health

This allows the user to determine whether the displayed sensor readings are recent and whether the monitoring system is operating normally.

12. View Historical Data
Step 15 — View Recorded Sensor Data

Sensor readings sent to the backend can be stored in the database.

The stored information may include:

Temperature
Humidity
Gas/smoke level
Motion status
Alarm status
Date and time of the reading

Historical data can be used to observe environmental changes and review previous alarm events.

13. Respond to an Alarm
Step 16 — Identify the Alarm Source

When an alarm is activated, check the dashboard to determine which sensor caused the alarm.

For example:

High Temperature:
Check the monitored environment for excessive heat.

High Gas/Smoke Level:
Check the area for smoke, gas, or other possible sources affecting the MQ-2 sensor.

Motion Detected:
Check the monitored area to determine whether the detected movement is expected.

Step 17 — Check the Physical Environment

After identifying the alarm source, inspect the monitored area and take the appropriate safety action.

If the alarm appears to be caused by an abnormal environmental condition, address the source before continuing normal operation.

14. Stopping the System
Step 18 — Stop the Dashboard

Close the EnviroGuard dashboard or web browser.

Step 19 — Stop the Backend

Stop the backend server through the terminal using the appropriate stop command.

Step 20 — Disconnect the Hardware

After the backend has been stopped, disconnect the Arduino Uno and ESP32 from the computer if the system is no longer required.

For safety, disconnect the power before modifying any wiring or sensors.

15. Basic Troubleshooting
Dashboard does not display sensor data

Check that:

The Arduino Uno is powered.
The sensors are properly connected.
The ESP32 is powered.
The ESP32 is receiving data from the Arduino.
The backend server is running.
The computer and ESP32 are connected to the correct network.
The API is accessible.
/health returns 503 Service Unavailable

Check that the backend server is running and that its required services, such as the database or device connection, are available.

ESP32 displays unreadable/garbled serial data

Check that the Serial Monitor baud rate matches the baud rate configured in the ESP32 program.

Sensor values appear incorrect

Check:

Sensor wiring.
Sensor power supply.
Ground connections.
Sensor configuration in the Arduino code.
Analog voltage levels, particularly for the MQ-2 when communicating with the ESP32.
16. Normal Operating Procedure

The complete normal operating sequence is:

1. Connect hardware
↓
2. Start Arduino Uno
↓
3. Start ESP32
↓
4. Verify ESP32 receives sensor data
↓
5. Start backend/API server
↓
6. Check /health endpoint
↓
7. Open EnviroGuard dashboard
↓
8. Monitor temperature and humidity
↓
9. Monitor gas/smoke level
↓
10. Monitor motion status
↓
11. Monitor alarm status
↓
12. Review historical data when needed