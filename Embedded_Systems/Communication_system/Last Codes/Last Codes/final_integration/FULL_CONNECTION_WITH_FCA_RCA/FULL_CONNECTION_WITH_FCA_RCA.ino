
#include <WiFi.h>
#include <HTTPClient.h>

#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)


#define FRONT_TRIGGER 23
#define FRONT_ECHO    22

/*
#define REAR_TRIGGER  5
#define REAR_ECHO     4
*/

/*
// WiFi credentials
const char* ssid = "OMARTAREK";            // WiFi SSID (network name)
const char* password =  "10ramadan"; // WiFi password

// Server details
const char* serverUrl = "https://178.32.101.106:3000/api/car-control"; // Server URL or IP address and port
*/

volatile long frontStart = 0, frontEnd = 0;
//volatile long rearStart  = 0, rearEnd  = 0;

volatile long frontDuration = 0;
//volatile long rearDuration = 0;

// Interrupt Service Routine for the front ultrasonic sensor
void IRAM_ATTR frontEchoISR() {
  if (digitalRead(FRONT_ECHO) == HIGH) {
    frontStart = micros();  // Echo started
  } else {
    frontEnd = micros();    // Echo ended
    frontDuration = frontEnd - frontStart;  // Duration of echo
  }
}

/*
// Interrupt Service Routine for the rear ultrasonic sensor
void IRAM_ATTR rearEchoISR() {
  if (digitalRead(REAR_ECHO) == HIGH) {
    rearStart = micros();  // Echo started
  } else {
    rearEnd = micros();    // Echo ended
    rearDuration = rearEnd - rearStart;  // Duration of echo
  }
}
*/

// Function to trigger the ultrasonic sensor
void triggerSensor(int triggerPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

void setup() {
  // Initialize UART0 (USB Serial) for communication with Serial Monitor
  Serial.begin(115200);

  // Initialize UART2 for communication with Tiva C
    Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);        // UART1 to Tiva C


/*
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Confirmation message to Serial Monitor
  Serial.println("ESP32 is ready to communicate with Tiva C.");
*/

  pinMode(FRONT_TRIGGER, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), frontEchoISR, CHANGE);  // Attach ISR

/*
  pinMode(REAR_TRIGGER, OUTPUT);
  pinMode(REAR_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(REAR_ECHO), rearEchoISR, CHANGE);  // Attach ISR
*/

}

char msg[] = {'1' , '0'};
int x = 0;// Define the character to send

void loop() {
  
 // Trigger both sensors
  triggerSensor(FRONT_TRIGGER);
  delayMicroseconds(50);  // Short delay to avoid overlap
//  triggerSensor(REAR_TRIGGER);
  delay(100);  // Wait for ISRs to complete

  Serial1.print(msg[x]); // Send the character
  Serial.print("Sent to Tiva C: ");
  Serial.println(msg[x]); // Confirm the sent character in Serial Monitor

  // Check if data is available from Tiva C
  if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String

    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
  }

/*
if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();  // Expected: "1", "2", "3", ...
      Serial.print("Received from backend: ");
      Serial.println(payload);

      // Check if it's a valid command (1-5)
    //  if (payload.length() == 1 && isDigit(payload[0])) {
        char command = payload[33];
        Serial2.print(command);  // Send to Tiva C
        Serial.print("Sent to Tiva C: ");
        Serial.println(command);
    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.reconnect();
  }


  // Read ack from Tiva (if any)
  if (Serial2.available()) {
    String receivedData = Serial2.readString();
    Serial.print("Received: ");
    Serial.println(receivedData);
  }
*/

  // Calculate distance in cm (speed of sound ≈ 0.034 cm/µs, divide by 2 for round trip)
  float frontDistance = frontDuration * 0.034 / 2;
//  float rearDistance  = rearDuration * 0.034 / 2;

  // Print distances
  Serial.print("Front Distance: ");
  Serial.print(frontDistance);
  Serial.print(" cm\n");

/*
  Serial.print("Rear Distance: ");
  Serial.print(rearDistance);
  Serial.println(" cm");
*/

   // Control the car based on ultrasonic sensor readings
  if (frontDistance < 100 ) 
    { 
      x = 1;
      Serial.println("front Object detected, stopping car.");
      Serial1.print(msg[x]);  // Send stop command to Tiva C 
    }
  else 
  {
    x = 0;
  }
  

/*
  if ( rearDistance < 30) 
    {
      Serial.println("back Object detected, stopping car.");
      Serial2.print('5');  // Send stop command to Tiva C 
   }
   
    else {
            Serial.println("the distances from front and back are fine");
    }
*/

  delay(500);
 } 
