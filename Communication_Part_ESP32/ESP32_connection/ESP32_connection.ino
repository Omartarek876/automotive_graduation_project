#define RXD2 16 // GPIO16 for RX (UART2)
#define TXD2 17 // GPIO17 for TX (UART2)

void setup() {
  // Initialize UART0 (USB Serial) for communication with Serial Monitor
  Serial.begin(115200);

  // Initialize UART2 for communication with Tiva C
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Confirmation message to Serial Monitor
  Serial.println("ESP32 is ready to communicate with Tiva C.");
}

void loop() {
  char message = '1'; // Define the character to send

  // Send the character to Tiva C via UART2
  Serial2.print(message); // Send the character
  Serial.print("Sent to Tiva C: ");
  Serial.println(message); // Confirm the sent character in Serial Monitor

  // Check if data is available from Tiva C
  if (Serial2.available()) {
    String receivedData = Serial2.readString(); // Read all available data as a String
/*
    // Check if "ack" is in the received data
    if (receivedData.indexOf("ack") != -1) {
      Serial.println("Received 'ack' from Tiva C!");
    } else {
      */
      Serial.print("Received: ");
      Serial.println(receivedData); // Print the received data for debugging
    
  }

  delay(1000); // Delay 1 second between transmissions
}
