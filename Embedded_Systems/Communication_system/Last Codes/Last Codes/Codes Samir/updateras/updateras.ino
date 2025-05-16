#include <Wire.h>               // I2C library for ESP32
#include <WiFi.h>               // WiFi library for ESP32
#include <HTTPClient.h>         // HTTP requests library
#include <WiFiClientSecure.h>   // Secure HTTPS client
#include <ArduinoJson.h>        // JSON parsing library

// ====== Configuration ======
#define I2C_ADDRESS 0x08  // I2C address of the ESP32 (should match Raspberry Pi's)

const String BACKEND_URL = "https://178.32.101.106:3000/api/camera-control";  // Backend HTTPS URL

volatile byte receivedFlag = 0;         // Flag received from Raspberry Pi
volatile bool newFlagReceived = false;  // True if new flag was received and needs to be sent to backend

byte lastSentToBackend = 255;           // Last flag sent to backend
byte lastSentToRaspberry = 255;         // Last flag sent to Raspberry Pi
bool newDataForRaspberry = false;       // Indicates if there’s new data to send to Raspberry Pi

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin("Msamir", "samir3111");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  // Initialize I2C as slave
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.println("ESP32 is ready.");
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
  if (newFlagReceived) {
    updateBackendStatus(receivedFlag);
    newFlagReceived = false;
  }

  getBackendFeatureStatus();
  delay(500);
}
