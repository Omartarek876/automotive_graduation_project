// Include necessary libraries
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

// Define constants
#define EEPROM_SIZE 100  // Maximum number of fingerprints to store
#define fingerRX 16      // Pin for receiving data (RX) from fingerprint sensor
#define fingerTX 17      // Pin for sending data (TX) to fingerprint sensor

// Initialize the serial and fingerprint sensor objects
HardwareSerial sensorSerial(2);                          // Use UART2 for communication
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);  // Create fingerprint object using sensorSerial


void setup() {
    Serial.begin(115200);                         // Initialize serial communication for debugging
    sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);  // Begin communication with fingerprint sensor at 57600 baud
    finger.begin(57600);                          // Initialize the fingerprint sensor with baud rate 57600
    
    // Verify if the fingerprint sensor is connected properly
    if (!finger.verifyPassword()) {
        Serial.println("Fingerprint sensor not found!");  // If sensor not found, show error
        while (1);  // Halt the program
    }
    
    EEPROM.begin(EEPROM_SIZE);   // Initialize EEPROM for storing fingerprints
    Serial.println("\n\nPlace your finger to enroll...");  // Prompt user to place their finger for enrollment
}

void loop() {
    int newID = getAvailableID();  // Get the next available ID for enrollment
    if (newID == -1) {
        Serial.println("Memory full! Cannot enroll more fingerprints.");  // If no available memory, show message
        while (1);  // Halt the program 
            }
    
    // Try enrolling the fingerprint and store it in EEPROM if successful
    if (enrollFingerprint(newID)) {
        EEPROM.write(newID - 1, 1);  // Store the enrolled fingerprint ID in EEPROM (index shifted by 1)
        EEPROM.commit();  // Commit the changes to EEPROM
        Serial.print("Fingerprint successfully enrolled with ID: ");
        Serial.println(newID);  // Print the ID of the successfully enrolled fingerprint
    } else {
        Serial.println("Fingerprint enrollment failed! Try again.");  // If enrollment fails, show message
    }
    delay(3000);  // Wait for 3 seconds before allowing the next enrollment
}


// Function to find the next available ID for storing fingerprints
int getAvailableID() {
    for (int i = 0; i < EEPROM_SIZE; i++) {  // Loop through the EEPROM
        if (EEPROM.read(i) != 1) {   // If the slot is not occupied (value not 1)
            return i + 1;  // Return the available ID (starting from 1)
        }
    }
    return -1;  // If no available slots, return -1
}


// Function to enroll a fingerprint and store it in the sensor
bool enrollFingerprint(int id) {
    int p = -1;
    Serial.println("Place your finger on the sensor...");  // Prompt user to place their finger
    while (p != FINGERPRINT_OK) {  // Wait for the sensor to capture a valid image
        p = finger.getImage();  // Capture fingerprint image
        delay(100);  // Wait for a short time before trying again 
        }
    
    p = finger.image2Tz(1);  // Convert the first image to template 1
    if (p != FINGERPRINT_OK) {  // If the image conversion fails
        Serial.println("Failed to process first fingerprint image.");
        return false;  // Return false indicating failure  
           }
    
    Serial.println("Remove your finger...");  // Prompt user to remove their finger
    delay(2000);  // Wait for 2 seconds
    
    Serial.println("Place the same finger again...");  // Prompt user to place the same finger again
    p = -1;
    while (p != FINGERPRINT_OK) {  // Wait for the second image
        p = finger.getImage();  // Capture second fingerprint image
        delay(100);  // Wait before trying again    
         }
    
    p = finger.image2Tz(2);  // Convert the second image to template 2
    if (p != FINGERPRINT_OK) {  // If the second image conversion fails
        Serial.println("Failed to process second fingerprint image.");
        return false;  // Return false indicating failure   
          }
    
    p = finger.createModel();  // Try to create a fingerprint model from both images
    if (p != FINGERPRINT_OK) {  // If the model creation fails
        Serial.println("Fingerprint images do not match.");
        return false;  // Return false indicating failure   
          }
    
    p = finger.storeModel(id);  // Store the created fingerprint model at the specified ID
    if (p == FINGERPRINT_OK) {  // If storing is successful
        Serial.println("Fingerprint stored successfully."); 
        return true;  // Return true indicating success 
    } else {  // If storing fails
        Serial.println("Failed to store fingerprint.");
        return false;  // Return false indicating failure
    }
}


