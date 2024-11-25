#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

Adafruit_MPU6050 mpu;

// SCL Of MPU6050 to A5
// SDA Of MPU6050 to A4


#define FLIP_THRESHOLD 45.0      // Roll angle threshold
#define CHECK_DELAY 3000         // Time delay for checking flip (3 seconds)
#define TIME_DELAY 500           // Delay between readings (ms)

unsigned long startTime = 0;  // Start time for flip detection
bool flipDetected = false;    // Flag for detecting flip

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit MPU6050 Flip Detection!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(8, OUTPUT); // Set LED pin as an output
  Serial.println("Initialization complete.");
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;

   float roll = atan2(ay, sqrt(ax * ax + az * az)) * 180 / M_PI; 
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / M_PI;

  Serial.print("Acceleration X: ");
  Serial.print(ax);
  Serial.print(", Y: ");
  Serial.print(ay);
  Serial.print(", Z: ");
  Serial.print(az);
  Serial.println(" m/s^2");

  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print("°, Pitch: ");
  Serial.print(pitch);
  Serial.println("°");

  if (abs(roll) > FLIP_THRESHOLD || abs(pitch) > FLIP_THRESHOLD) {
    if (!flipDetected) {
      flipDetected = true;
      startTime = millis(); // Start the timer
    }

    if (millis() - startTime >= CHECK_DELAY) {
      Serial.println("Car flipped!");
      digitalWrite(8, HIGH); // Turn on the LED
    }
  } else {
    flipDetected = false;
    digitalWrite(8, LOW); // Turn off the LED
  }

  Serial.println("");
  delay(TIME_DELAY);
}
