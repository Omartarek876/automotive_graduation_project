#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)

#define FRONT_TRIGGER 23
#define FRONT_ECHO    22

#define REAR_TRIGGER  19
#define REAR_ECHO     21


volatile long frontStart = 0 , frontEnd = 0;
volatile long rearStart  = 0, rearEnd  = 0;

volatile long frontDuration = 0;
volatile long rearDuration = 0;
float prevDistance = 0.0;

const int maxSpeed = 50;
const int minSpeed = 0;
int currentSpeed = 0; // starting speed
char send_speed =0;

char speeds[] = { '15' , '20' , '25' , '30' , '35' , '40' . '45' , '50' , '55' , '60'};
int speed_index = 0;
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
  Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);        // UART1 to Tiva C

  pinMode(FRONT_TRIGGER, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), frontEchoISR, CHANGE);  // Attach ISR

  pinMode(REAR_TRIGGER, OUTPUT);
  pinMode(REAR_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(REAR_ECHO), rearEchoISR, CHANGE);  // Attach ISR

}
void loop() { 
    // Trigger both sensors
  triggerSensor(FRONT_TRIGGER);
  delayMicroseconds(50);  // Short delay to avoid overlap
  triggerSensor(REAR_TRIGGER);
  delay(100);  // Wait for ISRs to complete



  char message1 = '1'; // Define the character to send

  Serial1.print(message1); // Send the character
  Serial.print("Sent to Tiva C: ");
  Serial.println(message1); // Confirm the sent character in Serial Monitor

    // Check if data is available from Tiva C
  if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String

    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
   
  }
  
  delay(500);


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


  if (frontDistance < prevDistance -15 ) {
    // Object is getting closer → decrease speed gradually
    speed_index--;
    Serial1.print(speeds[speed_index]); // Send the character
    if (currentSpeed < minSpeed) 
    {
        currentSpeed = minSpeed;
        send_speed = '0';
        Serial1.print(send_speed); // Send the character
    }
    Serial.println("Decreasing speed...");
  } 
  else if (frontDistance > prevDistance + 15) {
    // Object is moving away → increase speed gradually
    speed_index++;
    Serial1.print(speeds[speed_index]); // Send the character
    if (currentSpeed > maxSpeed) 
      {
        currentSpeed = maxSpeed;
        Serial1.print(speeds[9]); // Send the character
      }
    Serial.println("Increasing speed...");
  }
   else {
    // Same distance → maintain current speed
    Serial.println("Maintaining speed...");
  } 



     // Check if data is available from Tiva C
  if (Serial1.available()) {  
    String receivedData = Serial1.readString(); // Read all available data as a String
    Serial.print("Received: ");
    Serial.println(receivedData); // Print the received data for debugging
   
  }



        // Speed adjustment based on distance
        if (frontDistance < 100  & command == 1 ) {
              // Danger zone → stop immediately
              
              Serial1.print(speeds[0]); // Send the character
              Serial1.print('5');  // Send stop command to Tiva C
              Serial.println("front Object detected, stopping car.");
              Serial.print("Sent to Tiva C: 5 (forword)"); 
        } 
        if (rearDistance <= 100 & command == 2 )
        {     
              Serial1.print(speeds[0]); // Send the character
              Serial1.print('5');  // Send stop command to Tiva C 
              Serial.println("back Object detected, stopping car.");
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

  delay(2000);
}