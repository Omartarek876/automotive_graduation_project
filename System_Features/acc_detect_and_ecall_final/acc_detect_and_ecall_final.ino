#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>

Adafruit_MPU6050 mpu;
TinyGPSPlus gps;

#define RXD2 16
#define TXD2 17
HardwareSerial gpsSerial(2); // UART2 for GPS

#define FLIP_THRESHOLD 45.0
#define CHECK_DELAY 3000
#define TIME_DELAY 500

unsigned long startTime = 0;
bool flipDetected = false;


// Wi-Fi Credentials
const char* ssid = "Msamir";         // Replace with your Wi-Fi SSID
const char* password = "samir3111"; // Replace with your Wi-Fi password

// Server URL for mobile app
const char* serverUrl = "https://32b0c8230c16f2.lhr.life"; // Replace with your server URL

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("System Initialized!");

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
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;

  float roll = atan2(ay, sqrt(ax * ax + az * az)) * 180 / M_PI;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / M_PI;

  if (abs(roll) > FLIP_THRESHOLD || abs(pitch) > FLIP_THRESHOLD) {
    if (!flipDetected) {
      flipDetected = true;
      startTime = millis();
    }

    if (millis() - startTime >= CHECK_DELAY) {
      Serial.println("Car flipped!");
      sendGPSLocation();
    }
  } else {
    flipDetected = false;
  }

  // Update GPS data for future use
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  delay(TIME_DELAY);
}

void sendGPSLocation() {
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


      String payload = "{\"lat\":" + String(lat, 6) + ",\"lng\":" + String(lng, 6) + ",\"status\":\"accident\"}";


      int httpResponseCode = http.POST(payload);

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Error sending POST: ");
        Serial.println(http.errorToString(httpResponseCode).c_str());
      }

      http.end();
    } else {
      Serial.println("Wi-Fi not connected. Cannot send data.");
    }
  } else {
    Serial.println("Unable to get GPS location!");
  }
}
