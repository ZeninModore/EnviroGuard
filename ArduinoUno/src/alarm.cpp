#include "alarm.h"
#include <Arduino.h>


// UNO pin
const int BUZZER_PIN = 8;


// Loop time
unsigned long arduinoTime = 0;
unsigned long buzzerTime = 0;


// Buzzer state variable
bool buzzerState = false;


// Buzzer initialization
void initializeBuzzer(){
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}


// Buzzer function
void buzzer() {
    if (temperatureAlarm ||
        gasAlarm ||
        motionAlarm) {

        buzzerState = !buzzerState;
        digitalWrite(BUZZER_PIN, buzzerState);
    }
    else {
        buzzerState = false;
        digitalWrite(BUZZER_PIN, LOW);
    }
}