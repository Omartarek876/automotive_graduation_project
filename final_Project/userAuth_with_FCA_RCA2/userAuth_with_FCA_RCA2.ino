#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <ESP32Servo.h>

#define EEPROM_SIZE 100  
#define fingerRX 16       // Fingerprint sensor RX (ESP TX)
#define fingerTX 17       // Fingerprint sensor TX (ESP RX)

#define SERVO_PIN 19      // moved to avoid UART1 TX (was GPIO 5)
#define RED_LED_PIN 34     // can be changed as needed

#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)[/]

#define FRONT_TRIGGER 27
#define FRONT_ECHO    32

#define REAR_TRIGGER   23
#define REAR_ECHO      22



volatile long frontStart = 0 , frontEnd = 0;
volatile long rearStart  = 0, rearEnd  = 0;

volatile long frontDuration = 0;
volatile long rearDuration = 0;

float prevDistance = 0.0;

char pre_command = 0; 
char command;

//int counter = 0;
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
  doorServo.write(270);  
  delay(5000);  
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

  EEPROM.begin(EEPROM_SIZE);
  doorServo.attach(SERVO_PIN);  // Use a non-conflicting pin
  doorServo.write(90);  // Initial position


  if (!finger.verifyPassword()) {
    Serial.println("Fingerprint sensor not found!");
    while (1);
  }

  pinMode(FRONT_TRIGGER, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), frontEchoISR, CHANGE);  // Attach ISR

  pinMode(REAR_TRIGGER, OUTPUT);
  pinMode(REAR_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(REAR_ECHO), rearEchoISR, CHANGE);  // Attach ISR
  
  
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);

/* المشكلة ف كتر الباور اللي بيتاخد من ال ESP 
 * محتاجين نجرب ندي السيرفو باور من برا لان المشكلة فيه ولو مظبطش هنعمل حاجتين اول حاجة نصور السيستم لوحدوا تاني حاجة ف الفيديو الاساسي نشيل السيرفو عادي مش مشكلة كدا كدا هو موجود ك simulation 
 * 
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
  */
  
}


void loop() {
    // Trigger sensors
  triggerSensor(FRONT_TRIGGER);
  delayMicroseconds(50);
  triggerSensor(REAR_TRIGGER);
  delay(100);  // Wait for echo

  command = '6'; // Define the character to send , it should be the recieved command from backend 

  float frontDistance = frontDuration * 0.034 / 2;
  float rearDistance = rearDuration * 0.034 / 2;

 // it should be removed in the final running to reduce the delay 
  Serial.print("Front Distance: ");
  Serial.print(frontDistance);
  Serial.print(" cm\t");
  Serial.print("Rear Distance: ");
  Serial.println(rearDistance);
  delay(500);


 // Emergency stop logic
  if ( (command == '6' && frontDistance < 500) || (command == '7' && rearDuration < 500) )
  {
    Serial1.print('4');               // Send stop
    Serial.println("Obstacle detected! Sent: STOP");
   // pre_command = '5';
  }

  else if (frontDistance > 500){
    Serial1.print(command); // send the commands ( 5 , 6 , 7 , 8 , 9 )
    Serial.print("Command Sent to Tiva C: ");
    Serial.println(command); // Confirm the sent character in Serial Monitor

    if ( (frontDistance > 500 ) && ( frontDistance < 650 ) ) 
    {
      Serial1.print('0'); // speed 35
      Serial.print("Spped Sent to Tiva C: ");
      Serial.println(command); // Confirm the sent character in Serial Monitor
    }
    else if ( (frontDistance > 650 ) && ( frontDistance < 800 ) )
    {
      Serial1.print('1'); // speed 35
      Serial.print("Spped Sent to Tiva C: ");
      Serial.println(command); // Confirm the sent character in Serial Monitor
    }
    else if ( (frontDistance > 800 ) && ( frontDistance < 950 ) )
    {
      Serial1.print('2'); // speed 35
      Serial.print("Spped Sent to Tiva C: ");
      Serial.println(command); // Confirm the sent character in Serial Monitor
    }
    else 
    {
      Serial1.print('3'); // speed 35
      Serial.print("Spped Sent to Tiva C: ");
      Serial.println(command); // Confirm the sent character in Serial Monitor
    }
  }



/*
    if (pre_command != command) {
          Serial1.print(command);
          Serial.print("Sent to Tiva C: ");
          Serial.println(command);
          pre_command = command;
    }
*/

//  prevDistance = frontDistance;

    // Check if data is available from Tiva C
    if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String

    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
   
      }
    

 } 
