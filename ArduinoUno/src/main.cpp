#include <Arduino.h>
#include "sensors.h"
#include "alarm.h"

// Config
Config config = {
    true,
    true,
    false,
    40.0,
    300
};


void setup() {
    Serial.begin(9600);

    initializeSensors();
    initializeBuzzer();
}

void loop(){
    unsigned long currentTime = millis();
    
    if(currentTime - arduinoTime >= 3000){
        arduinoTime = currentTime;

        SensorData data = readSensors();

        dht22(data, config);
        mq2(data, config);
        pir(data);

        Serial.println("--------------------");
    };


    if(currentTime - buzzerTime >= 500){
        buzzerTime = currentTime;
        buzzer();
    };
}