#include <Wire.h>  
#include <ArduinoJson.h>        // JSON parsing library
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define I2C_ADDRESS 0x08

#define EEPROM_SIZE 100  
#define fingerRX 16
#define fingerTX 17
#define UART1_TX 5
#define UART1_RX 4

#define FRONT_TRIGGER 27
#define FRONT_ECHO    32
#define REAR_TRIGGER   23
#define REAR_ECHO      18

//#define REAR_ECHO      25 // aw 33, 18 , 26



//leds
#define Green_LED    13  //  LED

//#define Red_LED    14  // Built-in LED

const char* ssid = "OMARTAREK";
const char* password =  "123456789";

// Server URLs
const char* serverUrl = "https://178.32.101.106:3000/api/car-control"; // Server URL or IP address and port
const char* authUrl = "https://178.32.101.106:3000/api/hw-auth";         // For fingerprint auth



volatile byte i2c_flag = 0;
byte prevflag=0;
bool new_flag_received = false;

volatile long frontStart = 0 , frontEnd = 0;
volatile long rearStart  = 0, rearEnd  = 0;

volatile long frontDuration = 0;
volatile long rearDuration = 0;

float prevDistance = 0.0;

char backend_command;
char last_backend_command = 0;


//volatile byte flag = 0;  // To store the incoming flag value

char auth = 0;
int fingerprintID;

HardwareSerial sensorSerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&sensorSerial);


void sendStatusToBackend(byte status) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Accept all SSL certs

    HTTPClient http;
    http.begin(client, authUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"status\":" + String(status) + "}";
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Auth Status Sent, Response: ");
      Serial.println(response);
    } else {
      Serial.print("Auth Send Failed, Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected. Can't send auth status.");
  }
}


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

void IRAM_ATTR frontEchoISR() {
  if (digitalRead(FRONT_ECHO) == HIGH) {
    frontStart = micros();
  } else {
    frontEnd = micros();
    frontDuration = frontEnd - frontStart;
  }
}

void IRAM_ATTR rearEchoISR() {
  if (digitalRead(REAR_ECHO) == HIGH) {
    rearStart = micros();
  } else {
    rearEnd = micros();
    rearDuration = rearEnd - rearStart;
  }
}

void triggerSensor(int triggerPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

void receiveEvent(int bytesReceived) {
  while (Wire.available()) {
    i2c_flag = Wire.read();  
  }
}


void setup() {
  Serial.begin(115200);
  sensorSerial.begin(57600, SERIAL_8N1, fingerRX, fingerTX);
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);

Wire.begin(I2C_ADDRESS);       // ESP32 as I2C slave
Wire.onReceive(receiveEvent); // Register callback
Serial.println("I2C Ready to receive flags from Raspberry Pi.");

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


  EEPROM.begin(EEPROM_SIZE);

/*
  if (!finger.verifyPassword()) {
    Serial.println("Fingerprint sensor not found!");
    while (1);
  }
*/
  pinMode(FRONT_TRIGGER, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), frontEchoISR, CHANGE);
  pinMode(REAR_TRIGGER, OUTPUT);
  pinMode(REAR_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(REAR_ECHO), rearEchoISR, CHANGE);
  
    // Initialize LEDS
  pinMode(Green_LED, OUTPUT);
  digitalWrite(Green_LED, LOW);

  
  while (!auth)
  {
   fingerprintID = searchFingerprint();
   if (fingerprintID >= 0) {
    Serial.print("Access Granted! Fingerprint ID: ");
    Serial.println(fingerprintID); 
      sendStatusToBackend(1);  // Send 1 if fingerprint success
      digitalWrite(Green_LED, HIGH);   // Turn LED on


    auth = 1;
   } else {
    Serial.println("Access Denied! No match found.");
    sendStatusToBackend(0);  // Send 0 if fingerprint fail
    //digitalWrite(Red_LED, HIGH);   // Turn LED on


   }
   delay(3000);
  } // end of user auth process

  

}

void loop() {
  triggerSensor(FRONT_TRIGGER);
  delayMicroseconds(50);
  triggerSensor(REAR_TRIGGER);

  delay(100);

  float frontDistance = frontDuration * 0.034 / 2;
  float rearDistance = rearDuration * 0.034 / 2;

  Serial.print("Front Distance: "); 
  Serial.print(frontDistance);
  Serial.print(" cm\tRear Distance: ");
  Serial.println(rearDistance);
  delay(500);

if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpResponseCode = http.GET();


    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();
     backend_command = payload[35]; //da bs l hytshal
      Serial.print("payload=");   
      Serial.print(payload);  
             /*
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("JSON parse error: ");
      Serial.println(error.c_str());
      return;
    }

    const char* actionn = doc["carControl"]["action"];
    byte value = atoi(actionn);

    backend_command = value;
    Serial.println("backend_command=");  
    Serial.print(backend_command); 


*/                      
            Serial.print("backend_command=");  
            Serial.print(backend_command); 
                      
 
  
      // Filter for commands 6 or 7 if distance is too close
      if ((backend_command == '6' && frontDistance < 500 ) || ( backend_command == '7' && rearDistance < 500)) {
        
        Serial1.print('4');               // slow then stop
        Serial.println("Obstacle detected! Sent: STOP");
      
      }
      else
      {
        Serial1.print(backend_command);
        Serial.println("Sent to Tiva C: "); 
        Serial.print(backend_command);
        delay(500);


        if (frontDistance > 500)
        {
          Serial1.print('0');
          Serial.println("Speed Sent to Tiva C: 35 "); 
        }        
        else if (frontDistance > 650)
        {
          Serial1.print('1');
          Serial.println("Speed Sent to Tiva C: 40 ");
        }
        else if (frontDistance > 800)
        {
          Serial1.print('2');
          Serial.println("Speed Sent to Tiva C: 45 ");
        }
        else
        { 
        //  Serial1.print('3');
        //  Serial.println("Speed Sent to Tiva C: 50 ");
        }      
      } 
    } 

    else {
      Serial.print("HTTP Error: "); 
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.reconnect();
  }
    
  if (Serial1.available()) {
    String receivedData = Serial1.readString();
    Serial.print("Received: "); 
    Serial.println(receivedData);
  }


    Serial.print("Flag received from Raspberry Pi: ");
    Serial.print(i2c_flag);  
    
    switch (i2c_flag) {
      case 7: //Dms
      Serial1.print('4'); 
      while(1);
      break;
      case 8: //red
      Serial1.print('4'); 
      delay(3000);
      break;
      case 9: ///green   
      Serial1.print('6'); 

      break;
      case 10: //stop

      Serial1.print('4'); 
              delay(3000);

      break;
    }
     i2c_flag=0;

    }

 /*
  if (prevflag != i2c_flag)
    {
    switch (i2c_flag) {
      case 7: //Dms
      Serial1.print('4'); 
      while(1);
      break;
      case 8: //red
      Serial1.print('4'); 
      break;
      case 9: ///green
      break;
      case 10: //stop
      Serial1.print('4'); 
      break;
    }
    prevflag = i2c_flag;
    }
    */