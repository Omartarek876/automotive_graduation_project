#define UART1_TX 5        // UART1 TX pin (to Tiva RX)
#define UART1_RX 4        // UART1 RX pin (from Tiva TX)

/*
#define FRONT_TRIGGER 23
#define FRONT_ECHO    22

#define REAR_TRIGGER  19
#define REAR_ECHO     21
*/

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

int speeds[] = {'0' , '1' , '2' , '3' , '4' , '5'};
int speed_index = 0;


// ISR for front ultrasonic sensor
void IRAM_ATTR frontEchoISR() {
  if (digitalRead(FRONT_ECHO) == HIGH) {
    frontStart = micros();
  } else {
    frontEnd = micros();
    frontDuration = frontEnd - frontStart;
  }
}

// ISR for rear ultrasonic sensor
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

void setup() {
  Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);  // UART1 to Tiva C
  

  pinMode(FRONT_TRIGGER, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), frontEchoISR, CHANGE);

  pinMode(REAR_TRIGGER, OUTPUT);
  pinMode(REAR_ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(REAR_ECHO), rearEchoISR, CHANGE);
}

void loop() {
  // Trigger sensors
  triggerSensor(FRONT_TRIGGER);
  delayMicroseconds(50);
  triggerSensor(REAR_TRIGGER);
  delay(100);  // Wait for echo


  command = '6'; // Define the character to send

  float frontDistance = frontDuration * 0.034 / 2;
  float rearDistance = rearDuration * 0.034 / 2;

  Serial.print("Front Distance: ");
  Serial.print(frontDistance);
  Serial.print(" cm\t");
  Serial.print("Rear Distance: ");
  Serial.println(rearDistance);
  delay(500);


 // Emergency stop logic
  if (command == '6' && frontDistance < 300) {
    Serial1.print('5');               // Send stop
    Serial.println("Obstacle detected! Sent: STOP");
   // pre_command = '5';
  }

  if (frontDistance > 300){
    Serial1.print(command); // Send the character
    Serial.print("Sent to Tiva C: ");
    Serial.println(command); // Confirm the sent character in Serial Monitor

  }

/*
  // Adjust speed gradually based on front object movement
  else if (frontDistance < prevDistance - 20) {
    speed_index--;
    if (speed_index < 0) speed_index = 0;

   // itoa(speeds[speed_index], speedStr, 10);
    Serial1.print(speeds[speed_index]);
    Serial.println("Decreasing speed...");
  } 

  else if (frontDistance > prevDistance + 20) {
    speed_index++;
    if (speed_index > 5) speed_index = 5;

  //  itoa(speeds[speed_index], speedStr, 10);
    Serial1.print(speeds[speed_index]);
    Serial.println("Increasing speed...");
  } 

  else {
    Serial.println("Maintaining speed...");
  }
*/



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
