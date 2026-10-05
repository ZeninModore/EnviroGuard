#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>


// Define Config
struct Config {
    bool temperatureAlarmEnabled;
    bool gasAlarmEnabled;
    bool motionAlarmEnabled;

    float temperatureThreshold;
    int gasThreshold;
};


// Defines SensorData
struct SensorData
{
    float temperature;
    float humidity;

    int gasSmoke;
    int motion;
};


// Sensors function declaration
SensorData readSensors();
void dht22(SensorData data, Config config);
void mq2(SensorData data, Config config);
void pir(SensorData data);

void initializeSensors();

#endif