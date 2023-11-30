# Autonomous Self-Driving Car Embedded Systems Project by Team 78

Welcome to Team 78's repository for our cutting-edge embedded systems project – an autonomous self-driving car. In this project, we aim to harness the power of embedded systems to create a sophisticated and intelligent vehicle capable of navigating its environment without human intervention. Our team has meticulously designed and implemented various components, each contributing to the overall functionality of the autonomous car.

## Project Overview

### Flowcharts

We have developed detailed flowcharts for key components of our self-driving car, providing a visual representation of the logic and decision-making processes. These include flowcharts for barcode scanning, line tracing, magnetometer integration, motor control, and ultrasonic sensor utilization.

### Dataflow

Our dataflow diagrams illustrate the seamless interaction between different modules within the system. From barcode scanning and direction control to object direction detection and motor control, these diagrams provide insights into how information is processed and utilized for decision-making.

### Sequence Diagram

The sequence diagram offers a dynamic view of the interactions between various components during different scenarios. It showcases the flow of activities, ensuring a comprehensive understanding of the system's behavior during operation.

### Block Diagram

Our block diagram provides a high-level overview of the system architecture, highlighting the interconnections between different modules. This visual representation simplifies the understanding of the overall structure of our autonomous self-driving car.

## Project Components and Task Distribution

### Barcode Scanning - done by Benjamin

Implementing a barcode scanning system enables the car to identify and interpret barcodes, enhancing its ability to recognize and respond to specific markers in its environment.

### Line Tracing - done by Benjamin

The line tracing component ensures the car can follow predefined paths accurately, making it capable of navigating through designated routes with precision.

### Magnetometer - done by Jovian

By integrating a magnetometer, our self-driving car can sense and respond to magnetic fields, adding an extra layer of environmental awareness for improved navigation.

### Motor Control - done by Timothy and Peter

Efficient motor control is crucial for the car's movement and agility. Our motor control system ensures precise and responsive actions based on the input from various sensors.

### Ultrasonic Sensors - done by Zheng Wei (Sean)

Ultrasonic sensors play a pivotal role in detecting obstacles and objects in the car's vicinity, contributing to real-time decision-making to avoid collisions.

### Web Server in pico w - done by Zheng Wei (Sean)

Web server driver in raspberry pico w for displaying the map and and other information captured on the autonomous self driving car, like the barcode character scanned.

## Integration of Project Components

The integration was done by both Jovian and Benjamin.

## Flowcharts
**Barcode**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/13552d2f-2db4-4628-8bca-72aa8c6590be)

**Line tracing**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/277a9d11-a450-4150-8719-3a56efd52ccc)

**Magnetometer**
![image](https://github.com/xrando/Embedded_Systems_T78/blob/main/Diagrams/Flowcharts/magneto_flowchart.jpg?raw=true)

**Motor**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/1dc24185-64b5-4a8f-86f4-8dc0827a7d17)

**Ultrasonic**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/0e6b970f-4064-4791-af04-affa58c235aa)

--- 
## Dataflow
**Barcode scanning**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/79bd0ae1-340b-4cf8-b6e8-23599b218b5d)

**Direction control**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/24f9fd13-e789-4efd-91f1-60af7980efcb)

**Object direction**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/f229e278-162b-463d-8e99-4510977babb5)

**Motor**
![image](https://github.com/xrando/Embedded_Systems_T78/assets/56796695/7ba8cbdc-f0ba-4f2c-bfe1-fcca47dfd925)

---
## Sequence Diagram
![image](https://github.com/xrando/Embedded_Systems_T78/blob/main/Diagrams/sequence_diagram.jpg?raw=true)

## Block diagram
![image](https://github.com/xrando/Embedded_Systems_T78/blob/main/Diagrams/block_diagram.jpg?raw=true)

## How to run the program
- Use pico arm compiler to build the project folder.
- Go to project build directory and extract the uf2 file and run this on the raspberry pico-w on the autonomous car.


## References or links that we used
Initialization of magnetometer guide: <br> 
https://github.com/ControlEverythingCommunity/LSM303DLHC/blob/master/C/LSM303DLHC.c

Some readings about magnetometer: <br>
https://www.digikey.com/en/maker/projects/how-to-calibrate-a-magnetometer/50f6bc8f36454a03b664dca30cf33a8b
<br>
https://youtu.be/eqZgxR6eRjo?si=f-6YrOMycQwiiu0b


Web ui reference guide: <br>
https://www.youtube.com/watch?v=C_O0MXGBHsw

Barcode tracing driver documentation reference source: <br>
https://www.barcodefaq.com/1d/code-39/
<br>
https://www.dcode.fr/barcode-39#f1