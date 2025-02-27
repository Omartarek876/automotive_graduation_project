#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ESP32Servo.h>

#define EEPROM_SIZE 10  
#define fingerRX 16     
#define fingerTX 17     
#define SERVO_PIN 5     
#define RED_LED_PIN 4   
#define UART1_TX 2      
#define UART1_RX 4      

HardwareSerial sensorSerial(2);  
HardwareSerial uart1(1);  

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);
Servo doorServo;

void setup() {
    Serial.begin(115200);
    sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);
    uart1.begin(115200, SERIAL_8N1, UART1_RX, UART1_TX);

    pinMode(RED_LED_PIN, OUTPUT);
    digitalWrite(RED_LED_PIN, LOW);

    doorServo.attach(SERVO_PIN);
    doorServo.write(90);  

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

        uart1.write(6);  // Send flag 6 to Tiva C
        delay(100);  

        while (uart1.available()) {  
            char response = uart1.read();  
            Serial.print("Tiva C Response: ");  
            Serial.println(response);
        }

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
