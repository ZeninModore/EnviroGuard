#include <Arduino.h>

HardwareSerial unoSerial(2);

void setup() {
    // USB serial → PC
    Serial.begin(115200);

    // Uno UART → ESP32
    unoSerial.begin(9600, SERIAL_8N1, 16, 17);

    Serial.println("ESP32 started");
    Serial.println("Waiting for Arduino...");
}

void loop() {
    if (unoSerial.available()) {
        String message = unoSerial.readStringUntil('\n');

        Serial.print("Received from Uno: ");
        Serial.println(message);
    }
}