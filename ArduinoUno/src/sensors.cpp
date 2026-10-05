#include "sensors.h"
#include <DHT.h>

// UNO pins
const int DHT_PIN = 2;
const int PIR_PIN = 3;
const int MQ2_PIN = A0;


DHT dht(DHT_PIN, DHT22);


// Actual alarm state variables
bool temperatureAlarm = false;
bool gasAlarm = false;
bool motionAlarm = false;


// Initialize sensors
void initializeSensors(){
    dht.begin();
    pinMode(PIR_PIN, INPUT);
}


// Read all sensors
SensorData readSensors(){
    SensorData data;

    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    data.gasSmoke = analogRead(MQ2_PIN);
    data.motion = digitalRead(PIR_PIN);

    return data;
}


// DHT22
void dht22(SensorData data, Config config){
    if(isnan(data.temperature) ||
        isnan(data.humidity)){
        Serial.println("Failed to read DHT22");
        temperatureAlarm = false;
        return;
    };

    if(config.temperatureAlarmEnabled &&
        data.temperature >= config.temperatureThreshold){
        Serial.println("EXTREME HEAT");
        temperatureAlarm = true;
    }

    else{
        temperatureAlarm = false;
    };

    Serial.print("Temperature: ");
    Serial.println(data.temperature);

    Serial.print("Humidity: ");
    Serial.println(data.humidity);
}


// MQ2
void mq2(SensorData data, Config config){
    if(config.gasAlarmEnabled &&
        data.gasSmoke >= config.gasThreshold){
        Serial.println("EXTREME LEVEL");
        gasAlarm = true;
    }

    else{
        gasAlarm = false;
    }

    Serial.print("Gas/Smoke: ");
    Serial.println(data.gasSmoke);
}


// PIR
void pir(SensorData data){
    if(data.motion == HIGH) {
        Serial.println("MOTION DETECTED");
        motionAlarm = true;
    }

    else{
        Serial.println("No Motion Detected");
        motionAlarm = false;
    }
}