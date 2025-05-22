#include <Wire.h>

// Define the I²C address for the ESP32
#define I2C_ADDRESS 0x08

volatile byte flag = 0;  // To store the incoming flag value

// This function will be called when data is received from the master
void receiveEvent(int bytesReceived) {
  while (Wire.available()) {
    flag = Wire.read();  // Read the incoming byte
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_ADDRESS);        // Initialize as I²C slave with address 0x08
  Wire.onReceive(receiveEvent);   // Register receive event callback
  Serial.println("ESP32 is ready to receive data.");
}

void loop() {
  // Process the received flag (e.g., display, toggle an output, etc.)
  Serial.print("Received flag: ");
  Serial.println(flag);
  delay(500);  // Adjust delay as needed
}