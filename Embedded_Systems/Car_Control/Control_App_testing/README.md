# Control System for Advanced Automotive Safety Mechanisms

## Overview
This document provides a detailed overview of the control system responsible for car movement in the "Advanced Automotive Safety Mechanisms" project. The control system is implemented on the Tiva C board using the AUTOSAR architecture, ensuring high levels of modularity, maintainability, and efficiency. The communication between the Tiva C board and the ESP32 module is wired via UART, ensuring reliability in real-time vehicle control operations.

## System Architecture

The control system follows a modular architecture based on the **AUTOSAR (Automotive Open System Architecture)** framework. This structure provides clear separation between hardware abstraction, communication, and control logic. The system is divided into the following layers:

1. **Application Layer**: 
   - This layer contains the specific functionalities needed to drive the vehicle. It translates commands (e.g., move forward, stop, turn) into actions that can be executed by the hardware. 
   - For example, receiving input commands from the ESP32 and interpreting them to adjust motor speed or direction.

2. **Service Layer**: 
   - The service layer contains reusable components that support the operation of the Application Layer. This includes scheduling, timing, and interrupt management, which are critical for real-time control and response.
   - For instance, handling time-dependent operations like vehicle movement speed or sensor readings.

3. **Basic Software Layer (BSW)**: 
   - This layer includes drivers and modules that interface with the hardware. Each driver is responsible for controlling and managing specific hardware resources.
   - **Drivers**: The Tiva C board drivers in this system are implemented to communicate with peripherals and sensors. These include:
     - **Port Driver**: Controls the GPIO pins for external connections (e.g., motors, sensors).
     - **DIO (Digital Input/Output) Driver**: Manages digital signals sent to and received from the vehicle’s hardware components, including motors and sensors.
     - **SysTick Driver**: Provides system ticks for managing time-based events, like motor control and sensor polling.
     - **NVIC (Nested Vector Interrupt Controller) Driver**: Handles interrupts, enabling real-time responses to events like emergency stop signals or critical system alerts.
     - **UART Driver**: Enables communication between the Tiva C board and the ESP32 module for sending and receiving control commands.

4. **Hardware Abstraction Layer (HAL)**:
   - The HAL provides an abstraction for the low-level hardware functionalities. It ensures that the drivers in the BSW interact with the physical components of the Tiva C board (e.g., GPIO, timers, UART).

5. **Communication Layer**:
   - Communication between the Tiva C and ESP32 is managed via **UART** (Universal Asynchronous Receiver-Transmitter), which provides a fast, reliable, and simple method for sending commands and receiving status updates.
   - Commands from the ESP32, such as vehicle movement directions, are encoded and transmitted over UART to the Tiva C board, which decodes and executes the appropriate actions.

## Control System Workflow

The control system operates in real-time, processing incoming commands and adjusting the vehicle’s behavior accordingly. The workflow includes:

1. **Initialization**:
   - Upon system startup, the initialization routines set up the drivers (Port, DIO, SysTick, NVIC, UART) and configure the hardware components.
   - The UART communication is established between the Tiva C and ESP32 modules, ensuring that data can be sent and received.

2. **Receiving Commands**:
   - The ESP32 continuously sends control commands to the Tiva C board through the UART interface. These commands include movement instructions (e.g., forward, backward, left, right, stop).
   - The UART driver on the Tiva C board listens for incoming data, and once a valid command is received, it is processed by the control logic.

3. **Processing Commands**:
   - The control logic interprets the received commands and determines the necessary actions. For instance, a "forward" command might instruct the motors to rotate in a specific direction at a particular speed.
   - The DIO driver controls the GPIO pins to activate specific motor drivers or actuators based on the decoded command.

4. **Real-Time Adjustments**:
   - Using the SysTick driver, the control system ensures that the commands are executed at the correct time intervals. For example, if the car is moving forward, the system will continuously check motor speed and adjust it to maintain the desired movement.
   - The NVIC driver ensures that any interrupt, such as an emergency stop command or a sensor alert, will immediately halt the ongoing action and execute the necessary response.

5. **Feedback Loop**:
   - Although not fully implemented yet, in future stages, sensors (e.g., speed or obstacle detection sensors) may be incorporated to provide feedback to the control system. The feedback would allow real-time adjustments to improve safety and functionality.

## Driver Details

### 1. **Port Driver**
   - The Port Driver interfaces with the general-purpose input/output (GPIO) pins of the Tiva C board. It is responsible for configuring, reading, and writing to the pins used for controlling the motors, sensors, and actuators.
   - This driver allows flexibility in connecting external hardware components, such as motor drivers or sensors, to the Tiva C board.

### 2. **DIO Driver**
   - The Digital Input/Output (DIO) Driver enables interaction with digital devices. This driver is used to send digital signals to the car’s hardware, such as activating motors or receiving digital sensor inputs.
   - It ensures the proper timing and voltage levels needed for controlling the hardware.

### 3. **SysTick Driver**
   - The SysTick Driver provides a timebase for scheduling events. It helps in implementing time-dependent functionality like motor speed control or the periodic checking of system status.
   - It works by generating interrupts at regular intervals, which the system uses to update or check system states.

### 4. **NVIC Driver**
   - The Nested Vector Interrupt Controller (NVIC) Driver enables efficient management of interrupt-driven processes.
   - This allows the system to immediately respond to critical events like emergency stops, sensor errors, or safety breaches.

### 5. **UART Driver**
   - The UART Driver is responsible for communication between the Tiva C board and the ESP32 module.
   - It configures the UART interface, sets up baud rates, and ensures the data is transmitted and received correctly.
   - This driver also handles any data encoding or decoding that might be necessary to interpret the control commands.

## Setup and Testing

### Hardware Requirements
- **Tiva C board**: The primary control unit for interfacing with the car's hardware.
- **ESP32**: Responsible for sending control commands to the Tiva C board.
- **Wires and connectors**: For UART communication and hardware interfacing.
- **Motors/Actuators**: For vehicle movement control.

### Software Setup
1. Program the Tiva C board with the control firmware.
2. Set up the ESP32 to send UART commands (e.g., "move forward," "stop") to the Tiva C board.
3. Connect the necessary hardware components, such as motor drivers, to the Tiva C board GPIO pins.

### Testing
1. **Unit Testing**: Test each driver individually to ensure functionality, such as proper GPIO control and UART communication.
2. **Integration Testing**: Verify that the entire system functions correctly when integrated, ensuring commands from the ESP32 are properly interpreted and executed by the Tiva C board.
3. **Real-Time Testing**: Evaluate the responsiveness of the system by observing how quickly and accurately the car responds to commands in real-world conditions.

## Future Improvements

- **Wireless Communication**: Integrate wireless communication (e.g., Wi-Fi or Bluetooth) for remote control.
- **Sensor Integration**: Incorporate sensors (e.g., GPS, obstacle detection) to provide feedback for advanced safety features.
- **Enhanced Control Algorithms**: Improve the control algorithms for more complex maneuvers and adaptive vehicle control based on real-time data.

---
