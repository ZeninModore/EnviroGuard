#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


// WIFI CONFIGURATION

const char* WIFI_SSID = "A NET 2.4G";
const char* WIFI_PASSWORD = "JOL1IVY3";


// ESP32 UART

// ESP32 RX = GPIO16
// ESP32 TX = GPIO17

HardwareSerial unoSerial(2);

const int UNO_RX_PIN = 16;
const int UNO_TX_PIN = 17;


// WEB SERVER

WebServer server(80);


// SENSOR DATA

struct SensorData {

    float temperature;
    float humidity;

    int gas;

    bool motion;
    bool alarm;
};


// Current data
SensorData sensorData = {

    0.0,
    0.0,
    0,

    false,
    false
};


// COMMUNICATION STATE

String incomingMessage = "";

unsigned long lastReceivedTime = 0;

const unsigned long connectionTimeout = 10000;


// FUNCTION DECLARATIONS

void connectWiFi();

void readUnoData();

void parseUnoData(String message);

void setupWebServer();

void handleSensorData();

void handleHealth();


// SETUP

void setup() {

    Serial.begin(115200);


    // UART CONNECTION TO ARDUINO UNO

    unoSerial.begin(
        9600,
        SERIAL_8N1,
        UNO_RX_PIN,
        UNO_TX_PIN
    );


    Serial.println();
    Serial.println("================================");
    Serial.println("SmartLink IES - ESP32");
    Serial.println("================================");


    // WIFI

    connectWiFi();


    // WEB SERVER

    setupWebServer();


    Serial.println();
    Serial.println("ESP32 ready.");
}


// MAIN LOOP

void loop() {

    // Read data from Arduino
    readUnoData();


    // Process HTTP requests
    server.handleClient();
}


// WIFI

void connectWiFi() {

    Serial.println();

    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(WIFI_SSID);


    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );


    int attempts = 0;


    while (
        WiFi.status() != WL_CONNECTED &&
        attempts < 20
    ) {

        delay(500);

        Serial.print(".");

        attempts++;
    }


    Serial.println();


    if (WiFi.status() == WL_CONNECTED) {

        Serial.println("Wi-Fi connected.");

        Serial.print("ESP32 IP address: ");

        Serial.println(WiFi.localIP());

    } else {

        Serial.println("Wi-Fi connection failed.");

        Serial.println(
            "ESP32 will continue receiving Arduino data."
        );
    }
}


// READ ARDUINO DATA

void readUnoData() {

    while (unoSerial.available()) {

        char incomingChar = unoSerial.read();


        // ----------------------------------------------------
        // END OF MESSAGE
        // ----------------------------------------------------

        if (incomingChar == '\n') {

            incomingMessage.trim();


            if (incomingMessage.length() > 0) {

                Serial.print("Received from UNO: ");

                Serial.println(incomingMessage);


                parseUnoData(incomingMessage);


                lastReceivedTime = millis();
            }


            incomingMessage = "";
        }


        // ----------------------------------------------------
        // BUILD MESSAGE
        // ----------------------------------------------------

        else {

            incomingMessage += incomingChar;
        }
    }
}


// PARSE UNO DATA

void parseUnoData(String message) {

    /*
       Expected:

       TEMP=28.50,HUM=70.20,GAS=123,MOTION=0,ALARM=0
    */


    int startIndex = 0;


    while (startIndex < message.length()) {

        int commaIndex = message.indexOf(
            ',',
            startIndex
        );


        String part;


        if (commaIndex == -1) {

            part = message.substring(
                startIndex
            );

            startIndex = message.length();

        } else {

            part = message.substring(
                startIndex,
                commaIndex
            );

            startIndex = commaIndex + 1;
        }


        int equalsIndex = part.indexOf('=');


        if (equalsIndex == -1) {

            continue;
        }


        String key = part.substring(
            0,
            equalsIndex
        );


        String value = part.substring(
            equalsIndex + 1
        );


        key.trim();
        value.trim();


        // ----------------------------------------------------
        // TEMPERATURE
        // ----------------------------------------------------

        if (key == "TEMP") {

            sensorData.temperature =
                value.toFloat();
        }


        // ----------------------------------------------------
        // HUMIDITY
        // ----------------------------------------------------

        else if (key == "HUM") {

            sensorData.humidity =
                value.toFloat();
        }


        // ----------------------------------------------------
        // GAS
        // ----------------------------------------------------

        else if (key == "GAS") {

            sensorData.gas =
                value.toInt();
        }


        // ----------------------------------------------------
        // MOTION
        // ----------------------------------------------------

        else if (key == "MOTION") {

            sensorData.motion =
                value.toInt() == 1;
        }


        // ----------------------------------------------------
        // ALARM
        // ----------------------------------------------------

        else if (key == "ALARM") {

            sensorData.alarm =
                value.toInt() == 1;
        }
    }
}


// WEB SERVER SETUP

void setupWebServer() {

    // SENSOR ENDPOINT

    server.on(
        "/api/sensors",
        HTTP_GET,
        handleSensorData
    );


    // HEALTH ENDPOINT

    server.on(
        "/health",
        HTTP_GET,
        handleHealth
    );


    // NOT FOUND

    server.onNotFound([]() {

        server.send(
            404,
            "application/json",
            "{\"error\":\"Endpoint not found\"}"
        );
    });


    server.begin();


    Serial.println("HTTP server started.");
}


// SENSOR API

void handleSensorData() {

    bool unoOnline =
        millis() - lastReceivedTime <
        connectionTimeout;


    String json = "{";


    json += "\"temperature\":";
    json += String(sensorData.temperature, 2);

    json += ",";


    json += "\"humidity\":";
    json += String(sensorData.humidity, 2);

    json += ",";


    json += "\"gas\":";
    json += String(sensorData.gas);

    json += ",";


    json += "\"motion\":";
    json += sensorData.motion
        ? "true"
        : "false";

    json += ",";


    json += "\"alarm\":";
    json += sensorData.alarm
        ? "true"
        : "false";

    json += ",";


    json += "\"unoOnline\":";
    json += unoOnline
        ? "true"
        : "false";


    json += "}";


    server.send(
        200,
        "application/json",
        json
    );
}


// HEALTH API

void handleHealth() {

    bool unoOnline =
        millis() - lastReceivedTime <
        connectionTimeout;


    String json = "{";


    json += "\"status\":\"ok\"";

    json += ",";


    json += "\"wifi\":";

    json +=
        WiFi.status() == WL_CONNECTED
        ? "true"
        : "false";

    json += ",";


    json += "\"uno\":";

    json += unoOnline
        ? "true"
        : "false";


    json += "}";


    server.send(
        200,
        "application/json",
        json
    );
}