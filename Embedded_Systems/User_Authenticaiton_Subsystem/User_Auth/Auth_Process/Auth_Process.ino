#include <HardwareSerial.h>

#define RXD2 16  // ESP32 UART2 RX (connected to Tiva TX4)
#define TXD2 17  // ESP32 UART2 TX (connected to Tiva RX4)

HardwareSerial TivaSerial(2);  // Using UART2

void setup() {
    Serial.begin(115200);  // USB Serial for monitoring
    TivaSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);  // UART2 for Tiva C

    Serial.println("ESP32 is ready to receive bytes from Tiva C via UART2.");
}

void loop() {
    if (TivaSerial.available()) {  // Check if data is available
        uint8_t receivedByte = TivaSerial.read();  // Read one byte
        Serial.print("Received byte from Tiva C: ");
        Serial.println(receivedByte);  // Print byte as decimal value
        // Send acknowledgment byte back to Tiva C
        TivaSerial.write(0x06);  // ASCII ACK (0x06) or any chosen byte
    }

    delay(100);  // Short delay for stability
}
