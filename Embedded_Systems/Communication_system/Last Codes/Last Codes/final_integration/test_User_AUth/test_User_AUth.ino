#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ESP32Servo.h>

#define EEPROM_SIZE 100  
#define fingerRX 16       // Fingerprint sensor RX (ESP TX)
#define fingerTX 17       // Fingerprint sensor TX (ESP RX)
#define SERVO_PIN 18     // moved to avoid UART1 TX (was GPIO 5)

#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)[/]
 

int counter = 0;
char auth = 0;
int fingerprintID;

HardwareSerial sensorSerial(2);  // UART2 for fingerprint
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);
Servo doorServo;

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
  doorServo.write(270);  
  delay(5000);  
  doorServo.write(90);   
  Serial.println("Closing door...");
}

void setup() {
  doorServo.attach(SERVO_PIN);  // Use a non-conflicting pin
  EEPROM.begin(EEPROM_SIZE);

  Serial.begin(115200);
  sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);  // Fingerprint UART2
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);        // UART1 to Tiva C
  
  doorServo.write(90);  // Initial position
  
}

void loop() {  
  
    while (!auth)
  {
   fingerprintID = searchFingerprint();
   if (fingerprintID >= 0) {
    Serial.print("Access Granted! Fingerprint ID: ");
    Serial.println(fingerprintID); 
    openDoor();
    auth = 1;
   } else {
    Serial.println("Access Denied! No match found.");
   }
   delay(3000);
  } // end of user auth process

  
  char message1 = '1'; // Define the character to send

  Serial1.print(message1); // Send the character
  Serial.print("Sent to Tiva C: ");
  Serial.println(message1); // Confirm the sent character in Serial Monitor

  // Check if data is available from Tiva C
  if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String

    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
  }
  delay(2000);
}
