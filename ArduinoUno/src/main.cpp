#include <Arduino.h>
#include <DHT.h>
#include <SoftwareSerial.h>


// Pin config

const int DHT_PIN = 2;
const int PIR_PIN = 3;
const int MQ2_PIN = A0;

const int BUZZER_PIN = 8;
const int LED_PIN = 9;

// SoftwareSerial
// Arduino RX = D10
// Arduino TX = D11
const int ESP_RX_PIN = 10;
const int ESP_TX_PIN = 11;


// Sensor config

#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

SoftwareSerial espSerial(ESP_RX_PIN, ESP_TX_PIN);


// Config

struct Config {

    // Alarm enable/disable
    bool temperatureAlarmEnabled;
    bool gasAlarmEnabled;
    bool motionAlarmEnabled;

    // Thresholds
    float maxTemperature;
    int maxGas;

    // Timing
    unsigned long sensorInterval;
    unsigned long sendInterval;

    // Buzzer
    unsigned long buzzerOnTime;
    unsigned long buzzerOffTime;
};


Config config = {

    true,       // temperature alarm enabled
    true,       // gas alarm enabled
    true,       // motion alarm enabled

    35.0,       // maximum temperature
    1000,       // maximum gas value

    3000,       // read sensors every 3 seconds
    3000,       // send data every 3 seconds

    200,        // buzzer ON for 200ms
    300         // buzzer OFF for 300ms
};


// Sensor data

struct SensorData {

    float temperature;
    float humidity;

    int gas;
    bool motion;

    bool temperatureAlarm;
    bool gasAlarm;
    bool motionAlarm;

    bool alarm;
};


// Current sensor data
SensorData sensorData;


// Timers

unsigned long lastSensorRead = 0;
unsigned long lastDataSend = 0;

unsigned long lastBuzzerChange = 0;


// Buzzer state

bool buzzerState = false;


// Function declarations

SensorData readSensors();

void updateAlarmState();

void updateBuzzer();

void updateLED();

void sendSensorData();

void printSensorData();


// Setup

void setup() {

    Serial.begin(9600);

    espSerial.begin(9600);

    dht.begin();

    pinMode(PIR_PIN, INPUT);

    pinMode(MQ2_PIN, INPUT);

    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(LED_PIN, OUTPUT);


    digitalWrite(BUZZER_PIN, LOW);

    digitalWrite(LED_PIN, LOW);


    Serial.println();
    Serial.println("================================");
    Serial.println("SmartLink IES - Arduino Uno");
    Serial.println("================================");

    Serial.println("Starting sensors...");

    delay(1000);

    Serial.println("Arduino ready.");
}


// Main loop

void loop() {

    unsigned long currentTime = millis();


    // READ SENSORS

    if (currentTime - lastSensorRead >= config.sensorInterval) {

        lastSensorRead = currentTime;

        sensorData = readSensors();

        updateAlarmState();

        printSensorData();
    }


    // SEND DATA TO ESP32

    if (currentTime - lastDataSend >= config.sendInterval) {

        lastDataSend = currentTime;

        sendSensorData();
    }


    // BUZZER

    updateBuzzer();

    // LED

    updateLED();
}


// Read sensors

SensorData readSensors() {

    SensorData data;


    // DHT22
    data.temperature = dht.readTemperature();

    data.humidity = dht.readHumidity();


    // MQ-2
    data.gas = analogRead(MQ2_PIN);


    // PIR
    data.motion = digitalRead(PIR_PIN) == HIGH;


    // Handler invalid DHT reading

    if (isnan(data.temperature)) {

        data.temperature = -999;
    }

    if (isnan(data.humidity)) {

        data.humidity = -999;
    }


    // Default alarm states
    data.temperatureAlarm = false;
    data.gasAlarm = false;
    data.motionAlarm = false;

    data.alarm = false;


    return data;
}


// Alarm state

void updateAlarmState() {


    // Temperature

    if (
        config.temperatureAlarmEnabled &&
        sensorData.temperature != -999 &&
        sensorData.temperature >= config.maxTemperature
    ) {

        sensorData.temperatureAlarm = true;
    }


    // Gas

    if (
        config.gasAlarmEnabled &&
        sensorData.gas >= config.maxGas
    ) {

        sensorData.gasAlarm = true;
    }


    // Motion

    if (
        config.motionAlarmEnabled &&
        sensorData.motion
    ) {

        sensorData.motionAlarm = true;
    }


    // Overall Alarm

    sensorData.alarm =
        sensorData.temperatureAlarm ||
        sensorData.gasAlarm ||
        sensorData.motionAlarm;
}


// BUZZER

void updateBuzzer() {

    unsigned long currentTime = millis();


    // No alarm
    if (!sensorData.alarm) {

        buzzerState = false;

        digitalWrite(BUZZER_PIN, LOW);

        return;
    }


    // BUZZER ON

    if (buzzerState) {

        if (
            currentTime - lastBuzzerChange >=
            config.buzzerOnTime
        ) {

            buzzerState = false;

            lastBuzzerChange = currentTime;

            digitalWrite(BUZZER_PIN, LOW);
        }

        return;
    }


    // BUZZER OFF

    if (
        currentTime - lastBuzzerChange >=
        config.buzzerOffTime
    ) {

        buzzerState = true;

        lastBuzzerChange = currentTime;

        digitalWrite(BUZZER_PIN, HIGH);
    }
}


// LED

void updateLED() {

    if (sensorData.alarm) {

        digitalWrite(LED_PIN, HIGH);

    } else {

        digitalWrite(LED_PIN, LOW);
    }
}


// SEND DATA TO ESP32

void sendSensorData() {
    espSerial.print("TEMP=");
    espSerial.print(sensorData.temperature);

    espSerial.print(",HUM=");
    espSerial.print(sensorData.humidity);

    espSerial.print(",GAS=");
    espSerial.print(sensorData.gas);

    espSerial.print(",MOTION=");
    espSerial.print(sensorData.motion ? 1 : 0);

    espSerial.print(",ALARM=");
    espSerial.print(sensorData.alarm ? 1 : 0);

    espSerial.print("\n");
}


// ============================================================
// DEBUG OUTPUT
// ============================================================

void printSensorData() {

    Serial.println();
    Serial.println("---------- SENSOR DATA ----------");


    Serial.print("Temperature: ");

    if(sensorData.temperature == -999){

        Serial.println("ERROR");

    } else{

        Serial.print(sensorData.temperature);
        Serial.println(" C");
    }


    Serial.print("Humidity: ");

    if (sensorData.humidity == -999) {

        Serial.println("ERROR");

    } else {

        Serial.print(sensorData.humidity);
        Serial.println(" %");
    }


    Serial.print("Gas/Smoke: ");
    Serial.println(sensorData.gas);


    Serial.print("Motion: ");
    Serial.println(
        sensorData.motion ? "DETECTED" : "NONE"
    );


    Serial.print("Temperature Alarm: ");
    Serial.println(
        sensorData.temperatureAlarm ? "YES" : "NO"
    );


    Serial.print("Gas Alarm: ");
    Serial.println(
        sensorData.gasAlarm ? "YES" : "NO"
    );


    Serial.print("Motion Alarm: ");
    Serial.println(
        sensorData.motionAlarm ? "YES" : "NO"
    );


    Serial.print("Overall Alarm: ");
    Serial.println(
        sensorData.alarm ? "YES" : "NO"
    );


    Serial.println("--------------------------------");
}