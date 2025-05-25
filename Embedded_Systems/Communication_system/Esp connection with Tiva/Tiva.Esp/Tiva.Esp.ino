#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)

char command;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);        // UART1 to Tiva C

  }

  void loop() {
  
    command = '6'; //send command to tiva 

    Serial1.print(command); // send the commands ( 5 , 6 , 7 , 8 , 9 )
    Serial.print("Command Sent to Tiva C: ");
    Serial.println(command); // Confirm the sent character in Serial Monitor

    // Check if data is available from Tiva C
    if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String

    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
   
      }
    
 } 
