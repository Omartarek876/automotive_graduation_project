#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

#define EEPROM_SIZE 100  // Maximum number of fingerprints to store
#define fingerRX 4       // Change these pins to your preferred UART1 RX
#define fingerTX 2       // Change these pins to your preferred UART1 TX

HardwareSerial sensorSerial(1);  // Use UART1 now
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);

void setup() {
    Serial.begin(115200);
    sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);  // UART1 setup
    finger.begin(57600);
    
    if (!finger.verifyPassword()) {
        Serial.println("Fingerprint sensor not found!");
        while (1);
    }
    
    EEPROM.begin(EEPROM_SIZE);
    Serial.println("\n\nPlace your finger to enroll...");
}

void loop() {
    int newID = getAvailableID();
    if (newID == -1) {
        Serial.println("Memory full! Cannot enroll more fingerprints.");
        while (1);
    }
    
    if (enrollFingerprint(newID)) {
        EEPROM.write(newID - 1, 1);  // Store at index newID-1 to fit EEPROM indexing
        EEPROM.commit();
        Serial.print("Fingerprint successfully enrolled with ID: ");
        Serial.println(newID);
    } else {
        Serial.println("Fingerprint enrollment failed! Try again.");
    }
    delay(3000);
}

int getAvailableID() {
    for (int i = 0; i < EEPROM_SIZE; i++) {
        if (EEPROM.read(i) != 1) {
            return i + 1;  // IDs start from 1
        }
    }
    return -1;  // No available slots
}

bool enrollFingerprint(int id) {
    int p = -1;
    Serial.println("Place your finger on the sensor...");
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        delay(100);
    }
    
    p = finger.image2Tz(1);
    if (p != FINGERPRINT_OK) {
        Serial.println("Failed to process first fingerprint image.");
        return false;
    }
    
    Serial.println("Remove your finger...");
    delay(2000);
    
    Serial.println("Place the same finger again...");
    p = -1;
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        delay(100);
    }
    
    p = finger.image2Tz(2);
    if (p != FINGERPRINT_OK) {
        Serial.println("Failed to process second fingerprint image.");
        return false;
    }
    
    p = finger.createModel();
    if (p != FINGERPRINT_OK) {
        Serial.println("Fingerprint images do not match.");
        return false;
    }
    
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
        Serial.println("Fingerprint stored successfully.");
        return true;
    } else {
        Serial.println("Failed to store fingerprint.");
        return false;
    }
}
