// Motor control pins
const int motorPin1 = 25;  // change this to your desired pin
const int motorPin2 = 26;  // change this to your desired pin

// PWM properties
const int freq = 5000;
const int resolution = 8;

// PWM channels
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;

// Duty cycle = 1/3 of 255 ≈ 85
int dutyCycle = 85;

void setup() {
  // configure LED PWM for both LEDs
  ledcAttach(ledPin1, freq, resolution);
  ledcAttach(ledPin2, freq, resolution);

  ledcWrite(ledPin1, dutyCycle);
  ledcWrite(ledPin2, dutyCycle); // SAME duty cycle
}
 
void loop() {  

}
