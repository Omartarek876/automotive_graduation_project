#include <WiFi.h>
#include <HTTPClient.h>

#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)

#define FRONT_TRIGGER 23
#define FRONT_ECHO    22

#define REAR_TRIGGER  19
#define REAR_ECHO     21


// WiFi credentials
const char* ssid = "OMARTAREK";            // WiFi SSID (network name)
const char* password =  "10ramadan"; // WiFi password

// Server details
const char* serverUrl = "https://178.32.101.106:3000/api/car-control"; // Server URL or IP address and port


volatile long frontStart = 0, frontEnd = 0;
volatile long rearStart  = 0, rearEnd  = 0;

volatile long frontDuration = 0;
volatile long rearDuration = 0;

float prevDistance = 0.0;

char pre_command = 0; 
char command;

int speeds[] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60};
int speed_index = 0;


char speedStr[4]; // buffer to convert speed from int to char*
// Interrupt Service Routine for the front ultrasonic sensor
void IRAM_ATTR frontEchoISR() {
  if (digitalRead(FRONT_ECHO) == HIGH) {
    frontStart = micros();  // Echo started
  } else {
    frontEnd = micros();    // Echo ended
    frontDuration = frontEnd - frontStart;  // Duration of echo
  }
}

// Interrupt Service Routine for the rear ultrasonic sensor
void IRAM_ATTR rearEchoISR() {
  if (digitalRead(REAR_ECHO) == HIGH) {
    rearStart = micros();  // Echo started
  } else {
    rearEnd = micros();    // Echo ended
    rearDuration = rearEnd - rearStart;  // Duration of echo
  }
}

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
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);  // UART1 to Tiva C


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

  pinMode(FRONT_TRIGGER, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), frontEchoISR, CHANGE);  // Attach ISR

  pinMode(REAR_TRIGGER, OUTPUT);
  pinMode(REAR_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(REAR_ECHO), rearEchoISR, CHANGE);  // Attach ISR

}

void loop() {

if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();  // Expected: "1", "2", "3", ...
      Serial.print("Received from backend: ");
      Serial.println(payload);
      
         command = payload[33];
        Serial1.print(command);  // Send to Tiva C
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

    // Check if data is available from Tiva C
  if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String

    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
   
  }
   delay(500);

 // Trigger both sensors
  triggerSensor(FRONT_TRIGGER);
  delayMicroseconds(50);  // Short delay to avoid overlap
  triggerSensor(REAR_TRIGGER);
  delay(100);  // Wait for ISRs to complete


  // Calculate distance in cm (speed of sound ≈ 0.034 cm/µs, divide by 2 for round trip)
  float frontDistance = frontDuration * 0.034 / 2;
  float rearDistance  = rearDuration * 0.034 / 2;

  // Print distances
  Serial.print("Front Distance: ");
  Serial.print(frontDistance);
  Serial.print(" cm\t");

  Serial.print("Rear Distance: ");
  Serial.print(rearDistance);
  Serial.println(" cm");

// Emergency stop logic
  if ((command == '1' && frontDistance < 100) || (command == '2' && rearDistance < 100)) {
    itoa(speeds[0], speedStr, 10);    // Lowest speed
    Serial1.print(speedStr);          // Send speed
    Serial1.print('5');               // Send stop
    Serial.println("Obstacle detected! Sent: STOP");
    pre_command = '5';
  } 
  else {
  // Adjust speed gradually based on front object movement
  if (frontDistance < prevDistance - 15) {
    speed_index--;
    if (speed_index < 0) speed_index = 0;

    itoa(speeds[speed_index], speedStr, 10);
    Serial1.print(speedStr);
    Serial.println("Decreasing speed...");
  } 
  else if (frontDistance > prevDistance + 15) {
    speed_index++;
    if (speed_index > 9) speed_index = 9;

    itoa(speeds[speed_index], speedStr, 10);
    Serial1.print(speedStr);
    Serial.println("Increasing speed...");
  } 
  else {
    Serial.println("Maintaining speed...");
  }
   // Send command only if changed
          if (pre_command != command) {
            Serial1.print(command);
            Serial.print("Sent to Tiva C: ");
            Serial.println(command);
            pre_command = command;
    }
  }

   prevDistance = frontDistance;
  delay(2000);

 
}
