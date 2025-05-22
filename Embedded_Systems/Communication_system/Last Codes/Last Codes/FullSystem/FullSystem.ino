#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ESP32Servo.h>
#include <Wire.h>               // I2C library for ESP32
#include <WiFiClientSecure.h>   // Secure HTTPS client
#include <ArduinoJson.h>        // JSON parsing library

#define EEPROM_SIZE 100  
#define fingerRX 16       // Fingerprint sensor RX (ESP TX)
#define fingerTX 17       // Fingerprint sensor TX (ESP RX)
#define SERVO_PIN 18      // moved to avoid UART1 TX (was GPIO 5)
#define RED_LED_PIN 2     // can be changed as needed
#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)[/]

// ====== i2c Configuration ======
#define I2C_ADDRESS 0x08  // I2C address of the ESP32 (should match Raspberry Pi's)

const String BACKEND_URL = "https://178.32.101.106:3000/api/camera-control";  // Backend HTTPS URL

volatile byte receivedFlag = 0;         // Flag received from Raspberry Pi
volatile bool newFlagReceived = false;  // True if new flag was received and needs to be sent to backend

byte lastSentToBackend = 255;           // Last flag sent to backend
byte lastSentToRaspberry = 255;         // Last flag sent to Raspberry Pi
bool newDataForRaspberry = false;       // Indicates if there’s new data to send to Raspberry Pi

#define FRONT_TRIGGER 23
#define FRONT_ECHO    22

#define REAR_TRIGGER  19
#define REAR_ECHO     21

volatile long frontStart = 0, frontEnd = 0;
volatile long rearStart  = 0, rearEnd  = 0;

volatile long frontDuration = 0;
volatile long rearDuration = 0;

char pre_command = 0; 
char command;

float prevDistance = 0.0;

int speeds[] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60};
int speed_index = 0;
char speedStr[4]; // buffer to convert speed from int to char*

//int counter = 0;
char auth = 0;
int fingerprintID;

// WiFi credentials
const char* ssid = "OMARTAREK";            // WiFi SSID (network name)
const char* password =  "10ramadan"; // WiFi password

// Server details
const char* serverUrl = "https://178.32.101.106:3000/api/car-control"; // Server URL or IP address and port

HardwareSerial sensorSerial(2);  // UART2 for fingerprint
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);
Servo doorServo;


int searchFingerprint() {
  Serial.println("Place your finger on the sensor...");

  int p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    return finger.fingerID;
  }
  return -1;
}

void openDoor() {
  Serial.println("Opening door...");
  doorServo.write(120);  
  delay(1000);           
  doorServo.write(90);   

  delay(3000);  

  Serial.println("Closing door...");
  doorServo.write(60);   
  delay(1000);  
  doorServo.write(90);   
}

void indicateFailure() {
  digitalWrite(RED_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
}


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
  Serial.begin(115200);
  sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);  // Fingerprint UART2
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);        // UART1 to Tiva C

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
rial.println(WiFi.localIP());
  
  // Initialize I2C as slave
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  if (!finger.verifyPassword()) {
    Serial.println("Fingerprint sensor not found!");
    while (1);
  }
  EEPROM.begin(EEPROM_SIZE);

  pinMode(FRONT_TRIGGER, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), frontEchoISR, CHANGE);  // Attach ISR

  pinMode(REAR_TRIGGER, OUTPUT);
  pinMode(REAR_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(REAR_ECHO), rearEchoISR, CHANGE);  // Attach ISR
  
  
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);

  doorServo.attach(SERVO_PIN);  // Use a non-conflicting pin
  doorServo.write(90);  // Initial position
  
    while (!auth)
  {
   fingerprintID = searchFingerprint();
   if (fingerprintID >= 0) {
    Serial.print("Access Granted! Fingerprint ID: ");
    Serial.println(fingerprintID); 
    openDoor();
    auth = 1;
   } else {
    Serial.println("Access Denied! No match found.");
    indicateFailure();
   }
   delay(3000);
  } // end of user auth process
  
  //  pinMode(SERVO_PIN, OUTPUT);
    //                                  fffff
    digitalWrite(SERVO_PIN, LOW);

}

// Function triggered when Raspberry Pi writes data
void receiveEvent(int bytesReceived) {
  while (Wire.available()) {
    byte incomingFlag = Wire.read();
    if (incomingFlag != lastSentToBackend) {
      receivedFlag = incomingFlag;
      newFlagReceived = true;
      Serial.print("Received NEW flag from Raspberry Pi: ");
      Serial.println(receivedFlag);
    } else {
      Serial.print("Received SAME flag from Raspberry Pi, ignoring: ");
      Serial.println(incomingFlag);
    }
  }
}

// Function triggered when Raspberry Pi requests data
void requestEvent() {
  if (newDataForRaspberry) {
    Wire.write(lastSentToRaspberry);
    Serial.print("Raspberry Pi requested, sent NEW: ");
    Serial.println(lastSentToRaspberry);
    newDataForRaspberry = false;  // Mark data as sent
  } else {
    Wire.write(0);  // Send 0 if no new data
    Serial.println("Raspberry Pi requested, no new data, sent 0");
  }
}

// Send received flag to backend (if different from last sent)
void updateBackendStatus(byte value) {
  if (value == lastSentToBackend) {
    Serial.println("Same flag already sent to backend, skipping...");
    return;
  }

  lastSentToBackend = value;

  WiFiClientSecure secureClient;
  secureClient.setInsecure();

  HTTPClient http;
  http.begin(secureClient, BACKEND_URL);
  http.addHeader("Content-Type", "application/json");

  String jsonPayload = "{\"status\": " + String(value) + "}";

  int httpResponseCode = http.POST(jsonPayload);
  if (httpResponseCode > 0) {
    Serial.print("Sent to backend: ");
    Serial.println(jsonPayload);
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error sending to backend: ");
    Serial.println(httpResponseCode);
    Serial.println("Response payload: " + http.getString());
  }

  http.end();
}

// Get feature status from backend and prepare to send to Raspberry Pi if changed
void getBackendFeatureStatus() {
  WiFiClientSecure secureClient;
  secureClient.setInsecure();

  HTTPClient http;
  http.begin(secureClient, BACKEND_URL);

  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.print("Backend Response: ");
    Serial.println(payload);

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("JSON parse error: ");
      Serial.println(error.c_str());
      return;
    }

    const char* statusStr = doc["cameraControl"]["status"];
    byte value = atoi(statusStr);

    Serial.print("Extracted status: ");
    Serial.println(value);

    if (value != lastSentToRaspberry) {
      lastSentToRaspberry = value;
      newDataForRaspberry = true;  // Mark new data to send
      Serial.print("New status from backend, will send to Raspberry Pi: ");
      Serial.println(value);
    } else {
      Serial.print("Same status from backend, already sent before, skipping: ");
      Serial.println(value);
    }
  } else {
    Serial.print("Error getting backend status: ");
    Serial.println(httpResponseCode);
    Serial.println("Response payload: " + http.getString());
  }

  http.end();
}


void loop() {

if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpResponseCode = http.GET();
        
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

    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();  // Expected: "1", "2", "3", ...
      Serial.print("Received from backend: ");
      Serial.println(payload);

        command = payload[33];
        Serial1.print(command);  // Send to Tiva C
        Serial.print("Sent to Tiva C: ");
        Serial.println(command);
        
        // Read ack from Tiva (if any)
  if (Serial1.available()) { 
    String receivedData = Serial1.readString();
    Serial.print("Received: ");
    Serial.println(receivedData);
  }

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

    } else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.reconnect();
  }

   if (newFlagReceived) {
    updateBackendStatus(receivedFlag);
    newFlagReceived = false;
  }
  getBackendFeatureStatus();
 } 
