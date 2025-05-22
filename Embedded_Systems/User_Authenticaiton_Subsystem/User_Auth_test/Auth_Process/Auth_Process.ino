#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ESP32Servo.h>

#define EEPROM_SIZE 100  
#define fingerRX 4        // UART1 RX (from R307 TX)
#define fingerTX 2        // UART1 TX (to R307 RX)
#define SERVO_PIN 5       
#define RED_LED_PIN 18    // Changed from 4 to avoid UART conflict

HardwareSerial sensorSerial(1);  // Use UART1

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);
Servo doorServo;

void setup() {
    Serial.begin(115200);
    sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);

    pinMode(RED_LED_PIN, OUTPUT);
    digitalWrite(RED_LED_PIN, LOW);

    doorServo.attach(SERVO_PIN);
    doorServo.write(90);  // Neutral position

    if (!finger.verifyPassword()) {
        Serial.println("Fingerprint sensor not found!");
        while (1);
    }

    EEPROM.begin(EEPROM_SIZE);
    Serial.println("System Ready! Place your finger to authenticate.");
}

void loop() {
    int fingerprintID = searchFingerprint();
    if (fingerprintID >= 0) {
        Serial.print("Access Granted! Fingerprint ID: ");
        Serial.println(fingerprintID);

        openDoor();
    } else {
        Serial.println("Access Denied! No match found.");
        indicateFailure();
    }
    delay(3000);
}

int searchFingerprint() {
    Serial.println("Place your finger on the sensor...");
    
    int p = finger.getImage();
    if (p != FINGERPRINT_OK) return -1;
    
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) return -1;

    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
        return finger.fingerID;
    }
    return -1;
}

void openDoor() {
    Serial.println("Opening door...");
    doorServo.write(120);  
    delay(1000);           
    doorServo.write(90);   

    delay(3000);  

    Serial.println("Closing door...");
    doorServo.write(60);   
    delay(1000);  
    doorServo.write(90);   
}

void indicateFailure() {
    digitalWrite(RED_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(RED_LED_PIN, LOW);
}
