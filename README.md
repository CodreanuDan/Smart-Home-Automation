# 🚀 Smart Home System by Codreanu Dan

![Project Logo]("C:\Users\uig37216\Downloads\839adb88-0548-4771-9f96-ea52c0dce8bc.webp")

## Description
Smart Home System - Bachelor's Degree Project
This is my bachelor's degree project, where I aimed to create a functional smart home system with a particular focus on the thermal control and air quality modules, using a variety of sensors for these tasks. I wanted to make my project as interactive as possible, so I designed it with a lot of visualization and diversity in mind.

Project Overview
Starting from the bottom up, the information provided by the system can be accessed through three types of interfaces. Below, I will describe each main component of my project:

1. The Main Box Interface
The main box, which contains the MCU, sensors, and actuators, is equipped with two screens: one LCD and one OLED. These screens display information such as:

Network connection status
Sensor data
Climate control information
Navigation:
Navigation is done using a button. In the future, I plan to make the control box more interactive by adding more buttons, cursors, and a larger screen with a fully developed menu.

2. Sensor Connectivity
Sensor Communication: All the sensors I use are digital and communicate via the I2C protocol.
Air Quality Sensor: The air quality sensor is not compatible with my ESP32 microcontroller, so I connected it to an Arduino Uno board that communicates with the ESP32 via UART.
Software Architecture: The software architecture is modular and incorporates elements from the FreeRTOS operating system.
3. IoT Server Communication
Communication with the IoT server, which I'll describe next, is done via Wi-Fi using the MQTT (Message Queuing Telemetry Transport) protocol.

Initial Connection: The 'box' needs to be connected to the internet. To do this, the software on the main board activates Bluetooth Low Energy (BLE) and scans for a device to connect with and receive network information.
Mobile Phone Interaction: Using a mobile phone, you can connect to the board and transmit the credentials. Once connected, data transfer occurs via Wi-Fi.
Future Plans: I plan to integrate this BLE feature into my mobile app designed for this project and create a mechanism to store connection data from previous sessions.
4. Climate Control System
For the climate control system, I used a Peltier cell, which operates based on the thermoelectric effect, to create a heating/cooling unit.

Control Mechanism: The cell is controlled by an H-bridge that manages the current flow using two pairs of relays triggered by commands given by a finite state machine for climate control.
5. IoT Server and Raspberry Pi
Moving on to the Raspberry Pi board, this is where the IoT server is located, specifically the Mosquitto broker, which is designed for the MQTT protocol.

Python Program: A Python program runs on the server, acting as a client while controlling the data flow between the 'box' and Node-RED and Firebase.
Server Program Structure: The server program is structured in classes to make it easy to use. When the program is running, it:
Receives data from the box and sends it to the Node-RED Dashboard, which then sends it to Firebase.
Receives data from the Node-RED Dashboard or the mobile app via Firebase.
Main Tasks: The main idea behind this program revolves around two main tasks: establishing the connection and handling received data. These two tasks are synchronized and communicate using a JSON file.
6. Node-RED and Firebase Integration
Node-RED: Node-RED is a visual programming tool that is extremely useful for IoT projects. I used it to create nodes between the server and the database.
Dashboard Creation: Node-RED provides a custom tool to create a Dashboard from which we can access information and control the system.
Firebase: The best way to create a communication bridge between the server and my mobile app was by using a real-time database, so I chose Firebase from Google due to its ease of use and good performance.
7. Mobile App
As mentioned earlier, I created a simple Android app using Flutter. Currently, the app has basic functionalities, such as:

Climate control
Data visualization
These functionalities are achieved by reading and writing to the database. In the future, I plan to improve the app to make it more interactive.

Conclusion
I hope you will read this description in detail to get an idea of what I have accomplished here. Thank you for your attention!

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Contributing](#contributing)
- [License](#license)

## 🛠️ Installation
Step-by-step guide on how to get your project running locally.

```bash
# Install dependencies
npm install

# Run the app
npm start

