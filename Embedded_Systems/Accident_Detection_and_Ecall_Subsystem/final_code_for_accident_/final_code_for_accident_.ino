#include <Wire.h>
#include <math.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>

// GPS object for parsing GPS data
TinyGPSPlus gps;

// Pin definitions for UART communication with GPS
#define RXD2 16
#define TXD2 17

// Thresholds and delays
#define FLIP_THRESHOLD 60 // Roll angle threshold to detect a flip
#define CHECK_DELAY 3000  // Time to confirm flip in milliseconds
#define TIME_DELAY 500    // Delay between each loop iteration
#define flag_delay 10000
// Hardware serial for GPS communication
HardwareSerial gpsSerial(2); // UART2 for GPS

// Accelerometer and gyroscope data
float ax, ay, az;                // Accelerometer values
float RateRoll, RatePitch, RateYaw; // Gyroscope rates
float rollangle;                // Calculated roll angle
unsigned long startTime = 0;    // Timer for flip detection
unsigned long flag_time=0;
bool flipDetected = false;      // Flip detection flag
bool alertSent = false; 

// Wi-Fi credentials
const char* ssid = "Msamir";         // Wi-Fi SSID
const char* password = "samir3111"; // Wi-Fi password

// Server URL for sending GPS data
const char* serverUrl = "https://178.32.101.106:3000/api/location"; // Replace with your server URL

int counter_for_reset = 0;

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  Wire.setClock(400000); // Set I2C clock speed
  Wire.begin();          // Start I2C communication
  delay(250);

  // Initialize MPU6050 and GPS modules
  MPU6050_Set_Power_Mode();
  MPU6050_Init();
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2); 

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi!");
}

void loop() {
  // Read accelerometer and gyroscope data
  MPU6050_Read_Acc_And_Gyro_Data();

  // Calculate roll angle using accelerometer data
  rollangle = atan2(ay, az) * 180 / M_PI;

  // Print accelerometer and roll angle data for debugging
  Serial.print("ax=");
  Serial.print(ax);
  Serial.print(" |ay=");
  Serial.print(ay);
  Serial.print(" |az=");
  Serial.println(az);

  Serial.print("roll angle=");
  Serial.println(rollangle);

  // Check for car flip based on roll angle
  if (abs(rollangle) >= FLIP_THRESHOLD) {
    if (!flipDetected) {
      flipDetected = true;
      startTime = millis(); // Start timer
    }

    // Confirm flip if the condition persists for a specified duration
    if (millis() - startTime >= CHECK_DELAY && !alertSent) {
      Serial.println("Car flipped!");
      SendAccidentAlert(); // Send GPS location to the server
      alertSent = true;  // Prevent further alerts for the same accident
      flag_time=millis();
    }
  } else {
    flipDetected = false; // Reset flip detection
  }

  if (millis()- flag_time >= flag_delay)
  {
    alertSent = false;
  }
  
  // Update GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  delay(TIME_DELAY); // Wait before the next loop iteration

}

// Initialize MPU6050 accelerometer and gyroscope
void MPU6050_Init(void) {
  Wire.beginTransmission(0x68); // Address MPU6050
  Wire.write(0x1A);             // Configure DLPF
  Wire.write(0x04);             // Set DLPF to 20Hz
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x1C);             // Configure accelerometer range
  Wire.write(0x10);             // Set to ±8g
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x1B);             // Configure gyroscope range
  Wire.write(0x08);             // Set to ±500°/s
  Wire.endTransmission();
}

// Read accelerometer and gyroscope data from MPU6050
void MPU6050_Read_Acc_And_Gyro_Data(void) {
  // Read accelerometer data
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);             // Start reading at register 0x3B
  Wire.endTransmission(); 
  Wire.requestFrom(0x68, 6);
  int16_t ASX = Wire.read() << 8 | Wire.read();
  int16_t ASY = Wire.read() << 8 | Wire.read();
  int16_t ASZ = Wire.read() << 8 | Wire.read();

  // Read gyroscope data
  Wire.beginTransmission(0x68);
  Wire.write(0x43);             // Start reading at register 0x43
  Wire.endTransmission(); 
  Wire.requestFrom(0x68, 6);
  int16_t GyroX = Wire.read() << 8 | Wire.read();
  int16_t GyroY = Wire.read() << 8 | Wire.read();
  int16_t GyroZ = Wire.read() << 8 | Wire.read();

  // Scale raw data to meaningful values
  RateRoll = (float)GyroX / 65.5;
  RatePitch = (float)GyroY / 65.5;
  RateYaw = (float)GyroZ / 65.5;
  
  ax = (float)ASX / 4096;
  ay = (float)ASY / 4096;
  az = (float)ASZ / 4096;
}

// Set MPU6050 to active mode
void MPU6050_Set_Power_Mode(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Wake up MPU6050
  Wire.endTransmission();
}

// Send GPS location to the server in case of a detected accident
void SendAccidentAlert(void) {
  if (gps.location.isValid()) {
    float lat = gps.location.lat();
    float lng = gps.location.lng();

    Serial.print("Accident Location - Latitude: ");
    Serial.println(lat, 6);
    Serial.print("Accident Location - Longitude: ");
    Serial.println(lng, 6);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/json");

      String payload = "{\"latitude\":" + String(lat, 6) + ",\"longitude\":" + String(lng, 6) + ",\"status\":\"accedint\"}";

      // Retry mechanism for sending data
      int httpResponseCode = -1;
      int maxRetries = 3; // Number of retry attempts
      int retryCount = 0;

      while (retryCount < maxRetries) {
        httpResponseCode = http.POST(payload);

        if (httpResponseCode == 201) { // Success
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          break; // Exit the loop as the request was successful
        } else {
          Serial.print("Error sending POST (Attempt ");
          Serial.print(retryCount + 1);
          Serial.print("): ");
          Serial.println(http.errorToString(httpResponseCode).c_str());
          retryCount++;
          delay(1000); // Wait 1 second before retrying
        }
      }

      if (httpResponseCode != 201) {
        Serial.println("Failed to send data after multiple attempts.");
      }

      http.end();
    } else {
      Serial.println("Wi-Fi not connected. Cannot send data.");
    }
  } else {
    Serial.println("Unable to get GPS location!");
  }
}
