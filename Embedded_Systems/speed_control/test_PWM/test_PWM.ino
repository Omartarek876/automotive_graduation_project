// the number of the LED pins
const int ledPin1 = 16;  // LED1 on GPIO16
const int ledPin2 = 17;  // LED2 on GPIO17

// setting PWM properties
const int freq = 5000;
const int resolution = 8;
 
void setup() {
  // configure LED PWM for both LEDs
  ledcAttach(ledPin1, freq, resolution);
  ledcAttach(ledPin2, freq, resolution);
}
 
void loop() {
  // increase the LED brightness
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {   
    ledcWrite(ledPin1, dutyCycle);
    ledcWrite(ledPin2, dutyCycle); // SAME duty cycle
    delay(15);
  }

  // decrease the LED brightness
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {   
    ledcWrite(ledPin1, dutyCycle);
    ledcWrite(ledPin2, dutyCycle); // SAME duty cycle
    delay(15);
  }
}
