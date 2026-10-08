# **EnviroGuard: Smart Environmental Monitoring and Security System**

## **How to Use EnviroGuard**

EnviroGuard is a monitoring system that collects environmental and security information such as **temperature, humidity, gas/smoke level, and motion**. The collected sensor data is sent from the Arduino and ESP32 to the backend API, where it can be stored and displayed on the web dashboard.

---

## **Step 1 — Start the Hardware**

1. Connect the **Arduino Uno** to the computer using a USB cable.
2. Make sure the sensors are properly connected:

   * **DHT22** — Temperature and humidity
   * **PIR Sensor** — Motion detection
   * **MQ-2** — Gas and smoke detection
   * **Buzzer** — Alarm notification
3. Make sure the **ESP32** is connected and communicating with the Arduino through the appropriate connection.
4. Check that the required power and ground connections are properly connected.

**Important:** Check all wiring before powering the system to prevent incorrect readings or hardware problems.

---

## **Step 2 — Start the Arduino and ESP32**

1. Open the project in **Visual Studio Code with PlatformIO**.
2. Connect the Arduino Uno and ESP32 to the computer.
3. Upload the appropriate program to each board.
4. Open the **Serial Monitor**.
5. Make sure the serial communication speed is configured correctly, such as **9600 baud**.
6. Verify that sensor readings are being received.

The Arduino should collect the sensor readings and send the data through the communication system.

---

## **Step 3 — Start the Backend API**

1. Open a terminal in the backend project folder.
2. Start the backend server.
3. Wait for the server to finish starting.
4. Check the terminal for the server address and port.

The backend provides the API endpoints used to receive, process, store, and retrieve sensor information.

For example:

**Health Check:**
`GET /health`

A successful health check indicates that the backend and its required services are operating correctly.

---

## **Step 4 — Check the Database**

Before using the dashboard, make sure the database is available.

The system stores sensor information such as:

* Temperature
* Humidity
* Gas/smoke level
* Motion status
* Alarm status
* Date and time of the recorded reading

The database allows previous sensor readings to be retrieved and used for monitoring and historical information.

---

## **Step 5 — Open the EnviroGuard Dashboard**

1. Open the web browser.
2. Open the EnviroGuard dashboard.
3. Wait for the dashboard to load.
4. Check the system information and sensor displays.

The dashboard provides a visual interface for monitoring the current condition of the environment.

---

## **Step 6 — Monitor Temperature and Humidity**

The **Temperature** and **Humidity** sections display the latest readings collected by the DHT22 sensor.

The user should regularly check these values to determine whether the environment is within the expected condition.

For example:

**Temperature:** 28.5 °C
**Humidity:** 70.2 %

If the temperature reaches the configured alarm threshold, the system can activate the corresponding alarm.

---

## **Step 7 — Monitor Gas and Smoke Levels**

The **Gas/Smoke** section displays the reading collected by the MQ-2 sensor.

The user can monitor the value to determine whether the detected gas or smoke level has increased.

If the reading exceeds the configured threshold, the system can identify it as a potential environmental hazard and activate the gas alarm.

**Note:** MQ-2 readings are sensor values used for monitoring and should not be interpreted as an exact measurement of a specific gas concentration unless the sensor has been properly calibrated for that purpose.

---

## **Step 8 — Monitor Motion Detection**

The **Motion** section displays the current state of the PIR sensor.

The system can display whether motion is currently detected.

For example:

**Motion: Detected**

or

**Motion: No Motion**

If motion monitoring is enabled, the system can also trigger the configured alarm response.

---

## **Step 9 — Check the Alarm Status**

The dashboard provides an indication of the current **alarm status**.

The alarm can be triggered when a monitored condition exceeds its configured threshold.

Possible alarm sources include:

* **Temperature alarm**
* **Gas/smoke alarm**
* **Motion alarm**

The alarm configuration determines which conditions are allowed to activate the buzzer.

---

## **Step 10 — Check the Latest Update**

The dashboard displays the time and date associated with the latest sensor update.

The user can use this information to determine whether the displayed readings are current.

For example:

**Last Updated:** 11:33:07 AM
**Date:** 10/08/2026
**UTC:** +8

If the displayed time stops updating, the user should check the connection between the hardware, ESP32, backend API, and dashboard.

---

## **Step 11 — View Historical Sensor Data**

The system can store sensor readings in the database.

The user can use the available history functionality to review previous readings and observe changes in:

* Temperature
* Humidity
* Gas/smoke levels
* Motion events
* Alarm events

Historical data can help identify unusual environmental conditions or repeated security events.

---

## **Step 12 — Respond to an Alarm**

When an alarm is activated:

1. Check the dashboard to identify the condition that triggered the alarm.
2. Check the current sensor readings.
3. If the alarm is caused by **high temperature**, inspect the environment for an abnormal heat source.
4. If the alarm is caused by **gas/smoke**, check the area for smoke, gas leaks, or other possible hazards.
5. If the alarm is caused by **motion**, check the monitored area and determine whether the movement is expected.
6. Take the appropriate safety action based on the situation.

**Do not rely solely on EnviroGuard for emergency or life-safety decisions.**

---

## **Step 13 — Stop the System**

When monitoring is finished:

1. Stop the backend server.
2. Close the EnviroGuard dashboard.
3. Stop the Arduino and ESP32 programs if necessary.
4. Disconnect the USB cables.
5. If applicable, disconnect the external power supply.

Make sure the hardware is safely powered off before changing any wiring.

---

# **Basic System Usage Flow**

**1. Connect Hardware**
↓
**2. Start Arduino and ESP32**
↓
**3. Collect Sensor Readings**
↓
**4. Send Data to the Backend API**
↓
**5. Store Data in the Database**
↓
**6. Open EnviroGuard Dashboard**
↓
**7. Monitor Current Sensor Data**
↓
**8. Check Alerts and Alarm Status**
↓
**9. Review Historical Data When Needed**

---

# **Troubleshooting**

### **Dashboard does not load**

Check that the backend server is running and that the dashboard is using the correct API address.

### **Health status shows 503 Service Unavailable**

A **503 Service Unavailable** response generally means that the backend server is reachable, but one or more required services or dependencies are currently unavailable. Check the backend terminal and database connection.

### **Sensor values are not updating**

Check:

* Arduino USB connection
* ESP32 connection
* Sensor wiring
* Serial communication
* Baud rate
* Backend server
* API connection

### **Alarm does not activate**

Check that the corresponding alarm is enabled in the system configuration and that the sensor reading has exceeded its configured threshold.

### **Dashboard shows old data**

Check the **Last Updated** time. If it is not changing, check the communication between the hardware, API, database, and dashboard.

---

# **Normal Operating Sequence**

For normal operation, the recommended sequence is:

**Hardware → Arduino → ESP32 → Backend API → Database → Dashboard**

The user primarily interacts with the **EnviroGuard dashboard**, while the hardware and backend components operate in the background to collect, transmit, process, and store sensor data.
