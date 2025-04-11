#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ESP32Servo.h>

#define EEPROM_SIZE 100  
#define fingerRX 16       // Fingerprint sensor RX (ESP TX)
#define fingerTX 17       // Fingerprint sensor TX (ESP RX)
#define SERVO_PIN 18      // moved to avoid UART1 TX (was GPIO 5)
#define RED_LED_PIN 2     // can be changed as needed
#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)

HardwareSerial sensorSerial(2);  // UART2 for fingerprint
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);
Servo doorServo;

void setup() {
  Serial.begin(115200);
  sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);  // Fingerprint UART2
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);        // UART1 to Tiva C

  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);

  doorServo.attach(SERVO_PIN);  // Use a non-conflicting pin
  doorServo.write(90);  // Initial position

  if (!finger.verifyPassword()) {
    Serial.println("Fingerprint sensor not found!");
    while (1);
  }

  EEPROM.begin(EEPROM_SIZE);
  Serial.println("System Ready! Place your finger to authenticate.");
}

char auth = 0;
int fingerprintID;
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
    indicateFailure();
   }
   delay(3000);
  } // end of user auth process



} // end loop




