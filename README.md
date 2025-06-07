# Advanced Automotive Safety Mechanisms Connected with Mobile App

## Introduction

Road safety remains one of the most critical challenges in modern transportation.  
This project aims to significantly enhance automotive safety by integrating cutting-edge technologies such as Advanced Driver Assistance Systems (ADAS) and autonomous vehicle capabilities.

Our system focuses on addressing key safety concerns including delayed emergency responses after accidents, driver fatigue and authentication, vehicle theft prevention, and limited visibility during nighttime driving.  
By combining intelligent hardware modules and seamless communication with a mobile application, this project strives to create a safer driving environment.

Using a modular AUTOSAR architecture implemented on Tiva C microcontrollers and ESP32 communication modules, our solution delivers real-time accident detection, emergency handling, night vision support, secure driver authentication via fingerprint, and autonomous emergency control.

---

## Features

- **User Authentication:** Secure driver verification using fingerprint sensor and door lock control.  
- **Car Control:** Precise movement and speed control of the vehicle using PWM and command communication.  
- **Communication System:** Reliable data exchange between modules via ESP32 and UART.  
- **ADAS Features:** Advanced Driver Assistance Systems for enhanced driving safety.  
- **Night Vision:** Improved visibility during nighttime driving.  
- **Driver Monitoring System (DMS):** Monitoring driver condition to detect fatigue or distraction.  
- **Accident Detection & E-Call:** Automatic accident detection and emergency call notification.  
- **Mobile App:** Real-time monitoring and control through a dedicated mobile application.

---

## System Architecture

### User Authentication & Car Control Modules

#### Introduction

These modules implement the user authentication system and car movement control for the Advanced Automotive Safety project. The user authentication uses a fingerprint sensor and servo motor to secure access, while the car control module manages vehicle movement and speed based on commands received via ESP32.

#### Features

- **User Authentication:** Secure driver verification using R307 fingerprint sensor and servo motor door lock.  
- **Car Control:** Precise vehicle movement and speed control using PWM signals and UART command communication.

#### System Architecture Details

##### User Authentication System
- **Fingerprint Sensor (R307):** Captures and verifies the driver’s fingerprint.  
- **Servo Motor:** Controls the door lock, unlocking on successful authentication.  
- **Tiva C Microcontroller:** Interfaces with the fingerprint sensor and servo motor to manage authentication logic.

##### Car Control Module
- **Motor Drivers & PWM:** Controls direction (forward, backward, left, right, stop) and speed using PWM on PB6 (M0PWM0).  
- **UART Communication:** Receives movement commands from ESP32 via UART.  
- **Tiva C Microcontroller:** Processes commands and manages motor control signals.

---

## Hardware Requirements

- Tiva C LaunchPad board  
- R307 Fingerprint Sensor  
- Servo motor for door locking  
- DC motors for car movement  
- ESP32 module for communication  
- Proper wiring and connections according to pin assignments

---

## Software Components

- Fingerprint sensor driver and authentication code  
- Servo motor PWM control  
- Motor control drivers using PWM signals  
- UART communication driver  
- AUTOSAR-based layered architecture for modularity

---

## Installation / Setup

1. Connect the fingerprint sensor and servo motor to the Tiva C pins as per the hardware schematic.  
2. Connect DC motors and configure PWM on PB6 for speed control.  
3. Flash the combined authentication and movement firmware to the Tiva C board.  
4. Configure UART communication between Tiva C and ESP32 (matching baud rates).  
5. Enroll fingerprints using the provided fingerprint module code.  
6. Test door lock functionality via fingerprint authentication.  
7. Test movement commands sent from ESP32 and verify car response.

---

## Usage

- Place your finger on the sensor to authenticate.  
- On successful authentication, the servo motor unlocks the door.  
- Send movement commands (forward, backward, left, right, stop) from ESP32 over UART.  
- The car executes movement commands with speed control.

---

## Code Structure

- `auth_system/` — Fingerprint sensor and servo motor code  
- `car_movement/` — Movement drivers, PWM control, UART command handler

---

## Troubleshooting

- Ensure UART baud rates between ESP32 and Tiva C match.  
- Verify all wiring connections for fingerprint sensor and servo motor.  
- Check power supply levels to avoid resets during motor operation.  
- Confirm fingerprint sensor enrollment before testing authentication.

---

## Team Members

- Mariam Emad Fawzy  
- Mahmoud Ahmed Enany  
- Esraa Ashraf Fouad  
- Mahmoud Samir Mohamed

---

## Conclusion

This project presents a comprehensive approach to enhancing automotive safety through the integration of user authentication, advanced driver assistance systems, real-time accident detection, night vision, and seamless mobile app connectivity. The modular AUTOSAR architecture ensures maintainability and scalability, allowing future expansions and improvements.

We believe this system can significantly contribute to safer roads and more secure vehicles by addressing critical challenges in modern driving.

---

## Acknowledgments

We would like to thank our supervisor, Dr. Lamiaa Elrefaiee, for her invaluable guidance and support throughout this project. Thanks also to all team members for their dedication and hard work.

---

## Contact

For questions, collaborations, or further information, please contact:

- Omar Tarek Ibrahim — [omarrtarek74@gmail.com](mailto:omarrtarek74@gmail.com)  
- Esraa Ashraf Fouad  
- Mariam Emad Fawzy  
- Mahmoud Samir Mohamed  
- Mahmoud Ahmed Enany

---

© 2025 AutoSafe Team
