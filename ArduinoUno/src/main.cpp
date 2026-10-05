#include <Arduino.h>
#include <DHT.h>


// Default config
struct Config {
    bool temperatureAlarmEnabled;
    bool gasAlarmEnabled;
    bool motionAlarmEnabled;

    float temperatureThreshold;
    int gasThreshold;
};


Config config = {
    true,
    true,
    false,

    40.0,
    300
};


// Variables for buzzer
unsigned long previousBuzzerTime = 0;
bool buzzerState = false;


// Activation for buzzer
bool motionAlarm = false;
bool temperatureAlarm = false;
bool gasAlarm = false;


// Variables for main loop
unsigned long previousArduinoTime = 0;


// UNO Pins
const int DHT_PIN = 2;
const int PIR_PIN = 3;

const int MQ2_PIN = A0;
const int BUZZER_PIN = 8;

DHT dht(DHT_PIN, DHT22);


// Defining custom functions
struct SensorData
{
    float temperature;
    float humidity;

    int gasSmoke;
    int motion;
};


// Function declaration
SensorData readSensors();
void dht22(SensorData data);
void mq2(SensorData data);
void pir(SensorData data);
void buzzer();





// Main body
// Run once (Configs)
void setup(){
    Serial.begin(9600);
    dht.begin();

    pinMode(PIR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
}


// Loop (Update)
void loop(){
    unsigned long currentTime = millis();

    if(currentTime - previousArduinoTime >= 3000){
        previousArduinoTime = currentTime;
        SensorData data = readSensors();

        dht22(data);
        mq2(data);
        pir(data);

        Serial.println("--------------------");
    };
    

    if(currentTime - previousBuzzerTime >= 500){
        previousBuzzerTime = currentTime;
        buzzer();
    }
}





// Function definition
// Read sensors function
SensorData readSensors(){
    SensorData data;

    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    data.gasSmoke = analogRead(MQ2_PIN);
    data.motion = digitalRead(PIR_PIN);

    return data;
}


// DHT22 function
void dht22(SensorData data){
    if(isnan(data.temperature) || isnan(data.humidity)){
        Serial.println("Failed to read DHT22");
    };

    String tempAlertText;

    if(data.temperature >= 40){
        tempAlertText = "EXTREME HEAT";
        temperatureAlarm = true;
    }

    else if(data.temperature >= 35){
        tempAlertText = "DANGEROUS HEAT";
        temperatureAlarm = true;
    }
    
    else if(data.temperature <= 25){
        tempAlertText = "Colder than average";
    }

    else{
        tempAlertText = "Average Temperature";
        temperatureAlarm = false;
    };

    
    Serial.print("Temperature: ");
    Serial.println(data.temperature);

    Serial.print("Humidity: ");
    Serial.println(data.humidity);
    
}


// MQ2 function
void mq2(SensorData data){
    String mq2AlertText;
    
    if(data.gasSmoke >= 700){
        mq2AlertText = "EXTREME LEVEL";
        gasAlarm = true;
    }

    else if(data.gasSmoke >= 400){
        mq2AlertText = "DANGEROUS LEVEL";
        gasAlarm = true;
    }

    else if(data.gasSmoke >= 200){
        mq2AlertText = "Moderate Level";
        gasAlarm = false;
    }

    else{
        mq2AlertText = "Normal Level";
        gasAlarm = false;
    }

   
    Serial.print("Gas/Smoke: ");
    Serial.println(data.gasSmoke);
    Serial.println(mq2AlertText);
}


// PIR function
void pir(SensorData data){
    String pirAlertText;

    if(data.motion == HIGH){
        pirAlertText = "MOTION DETECTED";
        motionAlarm = true;
    }
    else{
        pirAlertText = "No Motion Detected";
        motionAlarm = false;
    }

    Serial.print("Motion: ");
    Serial.println(pirAlertText);
}


// Buzzer function
void buzzer(){
    if(temperatureAlarm || gasAlarm || motionAlarm){
        buzzerState = !buzzerState;

        digitalWrite(BUZZER_PIN, buzzerState);
    }

    else{
        digitalWrite(BUZZER_PIN, LOW);
    }    
}