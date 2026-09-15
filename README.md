# ESP32 Multi-Purpose Cleaning Robot

An embedded robotic cleaning system built using the **ESP32** that supports both
**Bluetooth-based manual control** and **autonomous cleaning using predefined paths**.

The system combines multiple cleaning mechanisms including **rotating brushes,
water spraying, and air blowing**, allowing it to perform cleaning operations on
solar panels and other flat surfaces.

## Project Overview

Traditional cleaning methods often require significant manual effort and can be
time-consuming, particularly for surfaces that need regular maintenance.

This project was developed as a cost-effective and adaptable robotic cleaning
platform capable of reducing human intervention while improving cleaning
efficiency.

The robot operates in two primary modes:

- **Manual Mode** – The user controls robot movement and cleaning mechanisms
  through a Bluetooth-connected mobile application.
- **Auto Path Mode** – The robot follows predefined cleaning paths while using
  sensor feedback for navigation and orientation.

## Key Features

- ESP32-based embedded control system
- Bluetooth communication with a mobile application
- Manual and autonomous operating modes
- Predefined cleaning paths
- MPU6050-based orientation sensing
- L298N-based DC motor control
- Rotating brush cleaning mechanism
- Water spraying system
- Air blower cleaning mechanism
- Relay-based actuator control
- Dual-battery power architecture

## Main Technologies

| Technology / Component | Purpose |
|---|---|
| ESP32 | Main system controller |
| Embedded C/C++ | Firmware development |
| Arduino IDE | Development and programming |
| Bluetooth | Wireless robot control |
| MPU6050 | Orientation and motion sensing |
| L298N | DC motor control |
| Relay Modules | Cleaning actuator switching |
| DC Motors | Robot movement |
| Water Pump | Water-based cleaning |
| DC Fan | Air-based cleaning |
| Rotating Brushes | Mechanical surface cleaning |

## System Concept

The ESP32 acts as the central controller of the robot. It receives commands from
a mobile application through Bluetooth and controls robot movement through an
L298N motor driver.

During automatic operation, the robot follows predefined paths while the
MPU6050 provides orientation information.

The cleaning mechanisms can operate using:

- Mechanical brushing
- Water-based cleaning
- Air-based cleaning

This combination allows the system to adapt to different cleaning requirements.

## Project Status

✅ Prototype developed  
✅ Manual Bluetooth control implemented  
✅ Automatic path operation implemented  
✅ Air, water, and brush cleaning mechanisms integrated  
✅ Research paper published  

More technical documentation, firmware, circuit diagrams, system architecture,
and demonstration material will be added to this repository.

## Publication

This project was published as:

**"Design and Implementation of a Multi-Purpose Cleaning System with Air and Water-Based Mechanisms"**

Journal of Emerging Technologies and Innovative Research (JETIR)  
Volume 12, Issue 6, June 2025

---

