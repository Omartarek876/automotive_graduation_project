#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ESP32Servo.h>


// i2c 
// send to backend
// test speeds 
// speeds and distances cases 
// alerts 
// test on the car 


#define EEPROM_SIZE 100  
#define fingerRX 16       // Fingerprint sensor RX (ESP TX)
#define fingerTX 17       // Fingerprint sensor TX (ESP RX)
#define SERVO_PIN 18      // moved to avoid UART1 TX (was GPIO 5)
#define RED_LED_PIN 2     // can be changed as needed
#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)[/]


/* pre ultrasonics pins0
#define FRONT_TRIGGER 18
#define FRONT_ECHO    19

#define REAR_TRIGGER  5
#define REAR_ECHO     4
*/

#define FRONT_TRIGGER 23
#define FRONT_ECHO    22

#define REAR_TRIGGER  21
#define REAR_ECHO     15


// WiFi credentials
const char* ssid = "OMARTAREK";            // WiFi SSID (network name)
const char* password =  "10ramadan"; // WiFi password

// Server details
const char* serverUrl = "https://178.32.101.106:3000/api/car-control"; // Server URL or IP address and port


volatile long frontStart = 0, frontEnd = 0;
volatile long rearStart  = 0, rearEnd  = 0;

volatile long frontDuration = 0;
volatile long rearDuration = 0;

char pre_command = 0; 
char command;


int counter = 0;
char auth = 0;
int fingerprintID;

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
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
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
    i8udigitalWrite(SERVO_PIN, LOW);

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

        if (frontDistance <= 100 & command == 1 )
        {     
              Serial.println("front Object detected, stopping car.");
              Serial1.print('5');  // Send stop command to Tiva C
              Serial.print("Sent to Tiva C: 5 (forword)"); 
        }
        
        if (rearDistance <= 100 & command == 2 )
        {  
              Serial.println("back Object detected, stopping car.");
              Serial1.print('5');  // Send stop command to Tiva C 
              Serial.print("Sent to Tiva C: 5 (backword)");
        }
        else 
        {     
            if (pre_command != command)
            {
              Serial.println("the distances from front and back are fine");
              Serial1.print(command);  // Send to Tiva C
              Serial.print("Sent to Tiva C: ");
              Serial.println(command);
              pre_command = command;
            }
        }
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
  if (Serial1.available()) { 
    String receivedData = Serial1.readString();
    Serial.print("Received: ");
    Serial.println(receivedData);
  }

 } 
