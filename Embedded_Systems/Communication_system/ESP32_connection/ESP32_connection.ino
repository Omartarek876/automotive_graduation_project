#define RXD1 4  // GPIO4 for RX (UART1)   B1 with 4 
#define TXD1 5  // GPIO5 for TX (UART1)   B0 with 5


 #define fingerRX 16     
 #define fingerTX 17 
 HardwareSerial sensorSerial(2);  


void setup() {

  sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);

  // Initialize UART0 (USB Serial) for communication with Serial Monitor
  Serial.begin(115200);

  // Initialize UART1 for communication with Tiva C
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);

  // Confirmation message to Serial Monitor
  Serial.println("ESP32 is ready to communicate with Tiva C.");
}

void loop() {

    delay(3000); // Delay 1 second between transmissions

  char message1 = '6'; // Define the character to send

  // Send the character to Tiva C via UART1
  Serial1.print(message1); // Send the character
  Serial.print("Sent to Tiva C: ");
  Serial.println(message1); // Confirm the sent character in Serial Monitor

  // Check if data is available from Tiva C
  if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String

    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
  }
/*
  delay(1000); // Delay 1 second between transmissions

    char message = '1'; // Define the character to send

  // Send the character to Tiva C via UART1
  Serial1.print(message); // Send the character
  Serial.print("Sent to Tiva C: ");
  Serial.println(message); // Confirm the sent character in Serial Monitor
*/
}
