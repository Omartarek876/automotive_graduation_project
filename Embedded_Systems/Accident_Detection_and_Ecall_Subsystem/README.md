# Accident Detection and Emergency Call System

## 1. System Overview
The **Accident Detection and Emergency Call System** is designed to detect vehicle rollover accidents and promptly send emergency alerts . The system integrates sensors and communication modules to monitor the vehicle's state and transmit critical data, such as location and vehicle status, to a mobile application backend. This backend processes the data and forwards it to the driver's relatives and emergency services, ensuring rapid assistance during accidents.

---

## 2. Components Used

### a. **MPU6050 (Accelerometer and Gyroscope)**
The MPU6050 sensor module combines a 3-axis accelerometer and a 3-axis gyroscope to monitor the vehicle’s orientation and detect rollovers based on the roll angle.

**Key Features:**
- **6 Degrees of Freedom (DOF)**: Combines accelerometer and gyroscope for comprehensive motion sensing.
- **I2C Interface**: Simple and efficient communication with the microcontroller.
- **High Accuracy**: Reliable data for motion tracking and rollover detection.

**Pinout:**

| Pin Name | Description                      |
|----------|----------------------------------|
| VCC      | Power Supply (3.3V/5V)           |
| GND      | Ground                           |
| SCL      | Serial Clock Line (I2C)          |
| SDA      | Serial Data Line (I2C)           |
| AD0      | I2C Address Selection            |
| INT      | Interrupt Output                 |
| XCL      | Auxiliary I2C Clock Line         |
| XDA      | Auxiliary I2C Data Line          |

---

### b. **GPS NEO-6M (GPS Module)**
The NEO-6M GPS module provides real-time location data, enabling the system to capture the vehicle's geographical coordinates during an accident.

**Key Features:**
- **High Sensitivity GPS Receiver**: Accurate location tracking.
- **UART Interface**: Easy communication with the microcontroller.
- **Output Data**: Provides latitude and longitude for precise tracking.

**Pinout:**

| Pin Name | Description                |
|----------|----------------------------|
| VCC      | Power Supply (3.3V/5V)     |
| GND      | Ground                     |
| TXD      | Transmit Data (UART)       |
| RXD      | Receive Data (UART)        |

---

### c. **ESP32 (Microcontroller)**
The ESP32 acts as the central processing unit, handling sensor data acquisition and communication with the mobile application backend via Wi-Fi.

**Key Features:**
- **High Processing Power**: Dual-core processor with extensive peripherals.
- **Wi-Fi and Bluetooth Support**: Built-in modules for seamless wireless communication.
- **HTTP Protocol Support**: Enables the system to send data to the backend server using standard HTTP requests.
- **Low Power Consumption**: Ideal for IoT and embedded applications.

---

## 3. System Functionality

### a. **Accident Detection Logic:**
The MPU6050 continuously measures the roll angle of the vehicle. A threshold value of 90 degrees is set for rollover detection. If the roll angle exceeds this threshold, the system considers the vehicle to have overturned.

### b. **Data Transmission:**
Once a rollover is detected:
1. **GPS Data**: The NEO-6M GPS provides the vehicle's real-time location.
2. **HTTP POST Request**: The ESP32 sends accident details (status, latitude, longitude) to the mobile application backend securely.

### c. **Communication Protocol:**
The ESP32 uses its Wi-Fi capability to establish an HTTP connection with the backend server, ensuring reliable and secure communication.

### d. **Notification to Relatives and Emergency Services:**
The backend server processes the data received from the ESP32 and triggers appropriate actions:
- Sends an emergency alert to the driver’s relatives with details of the accident and location.
- Notifies emergency services to facilitate a rapid response.

---

## 4. System Workflow
1. **Orientation Monitoring**: The MPU6050 measures the vehicle’s roll angle.
2. **Rollover Detection**: If the roll angle exceeds 90 degrees, the ESP32 identifies the event as a rollover.
3. **Location Retrieval**: The NEO-6M GPS provides real-time location data.
4. **Data Transmission**: The ESP32 sends the data to the backend via an HTTP POST request.
5. **Emergency Response**: The backend server processes the data and:
   - Sends accident details to the driver’s relatives.
   - Alerts emergency services with the vehicle’s location and status.

---

## 5. Advantages of the System
- **Real-Time Detection and Notification**: Ensures timely emergency response.
- **Accurate Tracking**: Provides precise location data for emergency services.
- **Comprehensive Alerts**: Relatives and emergency responders receive critical details simultaneously.
- **Cost-Effective Solution**: Uses affordable and scalable components.

---

## 6. Installation

### Hardware Setup:
1. Connect the MPU6050, GPS NEO-6M, and ESP32 according to the pinouts mentioned.
2. Ensure proper power supply for the components.

### Software Setup:
1. Clone the repository to your local machine.
2. Install the necessary librariesز
3. Upload the code to the ESP32 using the Arduino IDE.




