# 🚀 **Smart Home System by Codreanu Dan**

## 📝 **Project Overview**

This is my bachelor's degree project, where I aimed to create a functional smart home system with a particular focus on the thermal control and air quality modules, using a variety of sensors for these tasks. I wanted to make my project as interactive as possible, so I designed it with a lot of visualization and diversity in mind.

<p align="center">
    <img src="https://github.com/user-attachments/assets/63d97702-bdcb-4a8c-8567-4ba4dfdb76e1" alt="Diagrama proiect" height="400"/>
</p>
<p align="center"><em>Figure 1: Overall Project Diagram</em></p>

## 📑 **Table of Contents**
1. [The Main Box Interface](#1-the-main-box-interface)
2. [Sensor Connectivity](#2-sensor-connectivity)
3. [IoT Server Communication](#3-iot-server-communication)
4. [Climate Control System](#4-climate-control-system)
5. [IoT Server and Raspberry Pi](#5-iot-server-and-raspberry-pi)
6. [Node-RED and Firebase Integration](#6-node-red-and-firebase-integration)
7. [Mobile App](#7-mobile-app)

---

## 1. 🖥️ **The Main Box Interface**

The main box, which contains the MCU, sensors, and actuators, is equipped with two screens: one LCD and one OLED. These screens display information such as:

- 📶 Network connection status
- 📊 Sensor data
- 🌡️ Climate control information

<p align="center">
    <img src="https://github.com/user-attachments/assets/c6cb7e32-e763-4e55-b984-941ef46d4a73" alt="Main overview of the project" height="400"/>
</p>
<p align="center"><em>Figure 2: Overview of the Main Box Interface with LCD and OLED screens</em></p>

**Navigation:**

- Navigation is done using a button.
- **Future Plans:** Add more buttons, cursors, and a larger screen with a fully developed menu.

---

## 2. 📡 **Sensor Connectivity**

- **Sensor Communication:** All sensors are digital and communicate via the I2C protocol.
- **Air Quality Sensor:** Since the air quality sensor isn't compatible with the ESP32 microcontroller, it's connected to an Arduino Uno board that communicates with the ESP32 via UART.
- **Software Architecture:** Modular design incorporating elements from the FreeRTOS operating system.

<p align="center">
    <img src="https://github.com/user-attachments/assets/d99bb49f-a8fe-4e5d-8138-38b4f3381c85" alt="SchemaCablaje" height="400"/>
</p>
<p align="center"><em>Figure 3: Wiring Diagram for Sensor Connectivity</em></p>

<p align="center">
    <img src="https://github.com/user-attachments/assets/ed111ee5-4fe5-49bd-aa4a-35ff91f67a9c" alt="Arh_EMBEDED" height="400"/>
</p>
<p align="center"><em>Figure 4: Embedded Architecture for ESP32 System</em></p>

---

## 3. 🌐 **IoT Server Communication**

Communication with the IoT server is done via Wi-Fi using the MQTT (Message Queuing Telemetry Transport) protocol.

- **Initial Connection:** The 'box' activates Bluetooth Low Energy (BLE) to scan for a device and receive network credentials.
- **Mobile Phone Interaction:** Connect to the board using a mobile phone to transmit credentials. Once connected, data transfer occurs via Wi-Fi.
- **Future Plans:** Integrate the BLE feature into the mobile app and create a mechanism to store connection data from previous sessions.

<p align="center">
    <img src="https://github.com/user-attachments/assets/b2d7fd4a-d886-4f02-b6b0-83840afa9bc1" alt="BLE_WIFI_DIAGRAM" height="400"/>
</p>
<p align="center"><em>Figure 5: BLE and Wi-Fi Connection Diagram</em></p>

<p align="center">
    <img src="https://github.com/user-attachments/assets/6974e211-5331-4a14-a869-99ff0a50eaf9" alt="mqtt_COM" height="400"/>
</p>
<p align="center"><em>Figure 6: MQTT Communication Flow</em></p>

---

## 4. ❄️ **Climate Control System**

For the climate control system, I used a Peltier cell, which operates based on the thermoelectric effect, to create a heating/cooling unit.

- **Control Mechanism:** The cell is controlled by an H-bridge that manages the current flow using two pairs of relays triggered by commands from a finite state machine for climate control.

<p align="center">
    <img src="https://github.com/user-attachments/assets/c9afbe55-f0bc-4912-a40c-f86485e2c920" alt="Control_Racire" height="400"/>
</p>
<p align="center"><em>Figure 7: Climate Control System Using Peltier Cell</em></p>

<p align="center">
    <img src="https://github.com/user-attachments/assets/34b3ba98-33a6-4e9f-bace-8cd147296aef" alt="Racire_1sens" height="400"/>
</p>
<p align="center"><em>Figure 8: Peltier Cell Cooling Control Mechanism</em></p>

---

## 5. 🤖 **IoT Server and Raspberry Pi**

The Raspberry Pi hosts the IoT server, which includes the Mosquitto broker designed for the MQTT protocol.

- **Python Program:** A Python program runs on the server, acting as a client to control the data flow between the 'box,' Node-RED, and Firebase.
- **Server Program Structure:** The program is structured in classes to simplify usage. When running:
  - Receives data from the box and sends it to the Node-RED Dashboard, which then sends it to Firebase.
  - Receives data from the Node-RED Dashboard or mobile app via Firebase.
- **Main Tasks:** Establish the connection and handle received data, with synchronization through a JSON file.

<p align="center">
    <img src="https://github.com/user-attachments/assets/cafdab12-6d62-4a44-a736-8cd716085913" alt="Arhitectura_ServerMqtt" height="400"/>
</p>
<p align="center"><em>Figure 9: Server and MQTT Broker Architecture on Raspberry Pi</em></p>

<p align="center">
    <img src="https://github.com/user-attachments/assets/755e5ab9-2657-4e04-8afa-faa90c8830e9" alt="Organigrmama server" height="400"/>
</p>
<p align="center"><em>Figure 10: Server Program Flow and Data Handling</em></p>

---

## 6. 🛠️ **Node-RED and Firebase Integration**

- **Node-RED:** Used to create nodes between the server and the database, providing a visual programming interface for IoT projects.
- **Dashboard Creation:** Node-RED offers tools to create a Dashboard for accessing information and controlling the system.
- **Firebase:** A real-time database from Google chosen for its ease of use and performance, facilitating communication between the server and the mobile app.

<p align="center">
    <img src="https://github.com/user-attachments/assets/7382a9ef-49e0-4d41-b218-e6f3cce32dff" alt="Node_red_flow" height="400"/>
</p>
<p align="center"><em>Figure 11: Node-RED Flow for Data Handling</em></p>

<p align="center">
    <img src="https://github.com/user-attachments/assets/e1c1f03b-8998-4b37-8b7d-51277f96f994" alt="Node_RED_Dashboard" height="400"/>
</p>
<p align="center"><em>Figure 12: Node-RED Dashboard for Visual Control and Monitoring</em></p>

---

## 7. 📱 **Mobile App**

I created a simple Android app using Flutter. Currently, the app supports:

- 🌡️ Climate control
- 📊 Data visualization

**Future Plans:** Improve the app to make it more interactive.

<p align="center">
    <img src="https://github.com/user-attachments/assets/20f99217-32ea-4a46-968a-26487c2ae43b" alt="Mobile_App_1" height="400"/>
</p>
<p align="center"><em>Figure 13: Mobile App Arhitecture</em></p>

<p align="center">
    <img src="https://github.com/user-attachments/assets/260a3db9-1ca8-4309-81da-f0e74d0cdd3b" alt="Mobile_App_2" height="400"/>
</p>
<p align="center"><em>Figure 14: Mobile App for Visualization</em></p>

---

🎯 
I hope you read this description and also hope you like what you saw. Thank you for your attention!
