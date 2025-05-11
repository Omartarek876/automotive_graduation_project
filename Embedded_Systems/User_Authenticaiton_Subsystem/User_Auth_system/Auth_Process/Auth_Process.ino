#include <Adafruit_Fingerprint.h>  // Include library to work with fingerprint sensor
#include <HardwareSerial.h>         // Include library for hardware serial communication
#include <EEPROM.h>                 // Include library for EEPROM storage
#include <ESP32Servo.h>             // Include library to control servo motor

// Define constants for various pins and settings
#define EEPROM_SIZE 100  
#define fingerRX 16       // Fingerprint sensor RX connected to ESP32 TX
#define fingerTX 17       // Fingerprint sensor TX connected to ESP32 RX
#define SERVO_PIN 18      // Pin for controlling the door servo motor
#define RED_LED_PIN 2     // Pin for the red LED to indicate failure
#define UART1_TX 5        // UART1 TX pin for communication with Tiva C (TX)
#define UART1_RX 4        // UART1 RX pin for communication with Tiva C (RX)

HardwareSerial sensorSerial(2);  // Initialize UART2 for fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);  // Initialize fingerprint sensor object
Servo doorServo;  // Create servo object to control the door


// Function to search for a fingerprint match
int searchFingerprint() {
  Serial.println("Place your finger on the sensor...");  // Prompt user to place finger
  int p = finger.getImage();  // Capture image of the fingerprint
  if (p != FINGERPRINT_OK) return -1;  // If image capture fails, return -1
  p = finger.image2Tz();  // Convert the image to a template
  if (p != FINGERPRINT_OK) return -1;  // If image processing fails, return -1
  p = finger.fingerFastSearch();  // Search for the fingerprint in the database
  if (p == FINGERPRINT_OK) {  // If a match is found
    return finger.fingerID;  // Return the fingerprint ID
  }
  return -1;  // If no match, return -1
}


// Function to open and close the door using the servo motor
void openDoor() {
  Serial.println("Opening door...");  // Inform that the door is opening
  doorServo.write(120);  // Move servo to open position
  delay(1000);  // Wait for 1 second
  doorServo.write(90);   // Move servo back to neutral position
  delay(3000);  // Wait for 3 seconds
  Serial.println("Closing door...");  // Inform that the door is closing
  doorServo.write(60);   // Move servo to close position
  delay(1000);  // Wait for 1 second
  doorServo.write(90);   // Move servo back to neutral position
}


// Function to indicate failure with a red LED
void indicateFailure() {
  digitalWrite(RED_LED_PIN, HIGH);  // Turn on the red LED
  delay(1000);  // Wait for 1 second
  digitalWrite(RED_LED_PIN, LOW);   // Turn off the red LED
}


// Setup function runs once when the system starts
void setup() {
  Serial.begin(115200);  // Start serial communication for debugging
  sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);  // Start UART2 for fingerprint sensor
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);  // Start UART1 for communication with Tiva C
  pinMode(RED_LED_PIN, OUTPUT);  // Set red LED pin as output
  digitalWrite(RED_LED_PIN, LOW);  // Ensure the LED is off initially
  doorServo.attach(SERVO_PIN);  // Attach the servo motor to the defined pin
  doorServo.write(90);  // Set the servo to neutral position initially
  if (!finger.verifyPassword()) {  // Check if the fingerprint sensor is connected
    Serial.println("Fingerprint sensor not found!");  // If sensor is not found, display error
    while (1);  // Halt the program
  }
  EEPROM.begin(EEPROM_SIZE);  // Initialize EEPROM with defined size for fingerprint storage
  Serial.println("System Ready! Place your finger to authenticate.");  // System is ready for authentication
}


char auth = 0;  // Variable to track authentication status
int fingerprintID;  // Variable to store the fingerprint ID
// Main loop function, runs repeatedly
void loop() {
  while (!auth) {  // Keep checking for authentication until successful
    fingerprintID = searchFingerprint();  // Call the function to search for a matching fingerprint
    if (fingerprintID >= 0) {  // If a match is found
      Serial.print("Access Granted! Fingerprint ID: ");
      Serial.println(fingerprintID);  // Display fingerprint ID
      openDoor();  // Open the door
      auth = 1;  // Set authentication to true
    } else {  // If no match is found
      Serial.println("Access Denied! No match found.");  // Display failure message
      indicateFailure();  // Indicate failure with red LED
    }
    delay(3000);  // Wait for 3 seconds before trying again
  }
}


 