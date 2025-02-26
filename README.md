# Remote Controlled Robot Car 

## Overview
The gesture-controlled car uses motion input to naviate itself wirelessly through the ESP32-NOW communication protocal.
The system consists of a transmitter (controller) and reciever (car), utilizing the ESP-WROOM-32 microcontroller.
The project is designed to provide hands-free vehicle movement, making it useful for robotics, and automation applications.

## Components

### Transmitter (Controller)
* Microcontroller: ESP-WROOM-32
* Accelerometer/Gyroscope: MPU 6050
* Power Supply: 5V supply

### Reciever (car)
* Microcontroller: ESP-WROOM-32
* Motor Driver: L298N
* Motors: Standard DC motors
* Power supply: 5V for Microcontroller, 9V for Motors

## Key Takeways
1. ★ **NEW!**  ***Soldering Skills***  
I soldered pins onto the gyroscope module. This hands-on experience with circuit assembly and hardware connections is crucial for embedded systems roles that involve prototyping and hardware debugging.

2. ★ **NEW!** ***ESP-NOW Protocal***  
I used ESP-NOW, a low-latency, direct peer-to-peer communication protocol, in my project instead of Wi-Fi or Bluetooth to enable efficient data transfer between the ESP32 boards. Through this, I explored the differences between communication protocols, which is essential for designing reliable embedded systems.

3. ***Optimizing communication***  
Instead of continuously sending the gyroscope's position, I optimized communication by sending data only when a change was detected. This event-driven approach reduces power usage and network congestion, an important concept in embedded system design for efficient resource management.

4. ***Effective & Isolated testing***  
I systematically tested hardware components, software logic, and communication layers separately before integration. This structured testing approach ensures reliability and simplifies debugging, a key skill in embedded system development and validation.

5. ***Embedded System Debugging***  
I used serial monitoring to diagnose signal issues, improving my ability to debug embedded circuits and firmware

## Next steps 
* Add App Control - Implement Wi-fi, Bluetooth, or MQTT to control the car remotely via a web interface
* Optimize Firmware- Use FreeRTOS for multitasking and improve power efficiency
* Enchance Sensor processing
