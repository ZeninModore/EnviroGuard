#include <Arduino.h>
#include <DHT.h>

// UNO Pins
const int DHT_PIN = 2;
const int PIR_PIN = 3;

const int MQ2_PIN = A0;
const int BUZZER_PIN = 8;

DHT dht(DHT_PIN, DHT22);


// Run Once (Kind of like setting up configs)
void setup(){
    Serial.begin(9600);
    dht.begin();

    pinMode(DHT_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(MQ2_PIN, INPUT);

    pinMode(BUZZER_PIN, OUTPUT);
};


// Defining custom functions
struct SensorData
{
    float temperature;
    float humidity;

    int gasSmoke;
    int motion;
};


// Sensor Data
SensorData readSensors(){
    SensorData data;

    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    data.gasSmoke = analogRead(MQ2_PIN);
    data.motion = digitalRead(PIR_PIN);

    return data;
};


// Loops (Update)
void loop(){
    SensorData data = readSensors();

    Serial.println(data.temperature);
    Serial.println(data.humidity);

    Serial.println(data.gasSmoke);
    Serial.println(data.motion);

    delay(3000);
};