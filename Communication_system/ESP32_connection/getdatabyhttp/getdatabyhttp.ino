#include <WiFi.h>          // Include the WiFi library for managing WiFi connections
#include <HTTPClient.h>    // Include the HTTPClient library for making HTTP requests

// WiFi credentials
const char* ssid = "kevoo";            // WiFi SSID (network name)
const char* password = "Kevo24268&&"; // WiFi password

// Server details
const char* serverUrl = "http://192.168.1.3:4001/ctrl"; // Server URL or IP address and port

void setup() {
  Serial.begin(115200); // Initialize the serial communication at 115200 baud rate

  // Connect to WiFi
  WiFi.begin(ssid, password); // Start connecting to the WiFi network
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) { // Wait until the WiFi is connected
    delay(1000);       // Wait 1 second between retries
    Serial.print("."); // Print a dot to indicate progress
  }
  Serial.println("\nWiFi connected!"); // Print a success message once connected

  // Print local IP address
  Serial.print("Local IP: ");        // Print the prefix for the local IP address
  Serial.println(WiFi.localIP());    // Display the local IP address of the ESP32
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() == WL_CONNECTED) { // Verify that the WiFi connection is still active
    // Create an HTTPClient object
    HTTPClient http; // Instantiate the HTTPClient to manage HTTP requests

    // Send HTTP GET request to fetch the last command
    http.begin(serverUrl);             // Initialize an HTTP GET request to the server
    int httpResponseCode = http.GET(); // Execute the GET request and store the response code

    // Check if the request was successful
    if (httpResponseCode == HTTP_CODE_OK) { // If the response code is 200 (success)
      // Read the response (last command)
      String payload = http.getString(); // Get the response body as a string
      Serial.print("Last command: ");    // Print a prefix for the command
      Serial.println(payload);          // Display the last command received from the server
    } else {
      // Print an error message if the request fails
      Serial.print("Error fetching last command. HTTP code: "); // Print an error prefix
      Serial.println(httpResponseCode); // Display the HTTP response code
    }

    // Close the connection
    http.end(); // Free resources and close the HTTP connection
  } else {
    // If WiFi is disconnected, attempt to reconnect
    Serial.println("WiFi disconnected. Reconnecting..."); // Print a reconnection message
    WiFi.reconnect();                                    // Try to reconnect to the WiFi network
  }

  delay(50); // Add a small delay before the next loop iteration
}
