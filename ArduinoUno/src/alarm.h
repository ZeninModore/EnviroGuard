#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>


// Alarm states
extern bool temperatureAlarm;
extern bool gasAlarm;
extern bool motionAlarm;


// Loop time
extern unsigned long arduinoTime;
extern unsigned long buzzerTime;


// Buzzer declaration
void buzzer();
void initializeBuzzer();

#endif