# Hardware Architecture

This directory contains the hardware design and architecture of the
Multi-Purpose Cleaning Robot.

The system follows a modular architecture in which the ESP32 acts as the
central controller and coordinates motion, sensing, cleaning actuators,
communication, and power management.

## System Architecture

The hardware is divided into the following major subsystems:

### 1. Control Unit

The **ESP32** is the main controller of the robot.

It is responsible for:

- Receiving Bluetooth commands from the mobile application
- Controlling robot movement
- Processing MPU6050 sensor data
- Controlling cleaning mechanisms
- Executing predefined paths in Auto Path Mode

### 2. Motion Control

Robot movement is controlled using an **L298N motor driver**.

The motor driver receives control signals from the ESP32 and drives the
DC motors used for robot movement.

Supported movements include:

- Forward
- Backward
- Left
- Right
- Stop

### 3. Cleaning Mechanisms

The robot combines multiple cleaning methods.

#### Rotating Brushes

Rotating brushes provide mechanical cleaning by scrubbing dust and dirt
from the surface.

#### Water Cleaning

A DC water pump and vinyl tube are used to spray water onto the surface
for wet cleaning.

#### Air Cleaning

A DC fan is used as an air blower to remove loose dust and assist in
drying the cleaned surface.

The cleaning devices are controlled using relay modules connected to the
ESP32.

### 4. Orientation Sensor

The system uses an **MPU6050** motion sensor.

The MPU6050 contains:

- 3-axis accelerometer
- 3-axis gyroscope

It provides motion and orientation information that assists the robot
during automatic navigation.

### 5. Communication

The ESP32 communicates with a mobile application using **Bluetooth**.

The Bluetooth interface allows the user to:

- Control robot movement in Manual Mode
- Control cleaning mechanisms
- Start automatic cleaning operations

### 6. Power Management

The robot uses a dual-battery power architecture.

One battery powers the control electronics including:

- ESP32
- MPU6050
- Relay modules

A buck converter is used to provide a regulated supply to the sensitive
electronics.

The second battery powers higher-load components such as:

- DC motors
- Water pump
- Air blower

Separating the power sources helps reduce loading on the controller
supply and improves system stability.

## Main Hardware Components

| Component | Function |
|---|---|
| ESP32 | Main controller |
| L298N Motor Driver | DC motor control |
| DC Motors | Robot movement |
| MPU6050 | Orientation and motion sensing |
| Relay Modules | Switching cleaning mechanisms |
| Water Pump | Water-based cleaning |
| Vinyl Tube | Water delivery |
| DC Fan | Air-based cleaning |
| Rotating Brushes | Mechanical cleaning |
| MP1495 Buck Converter | Voltage regulation |
| Batteries | System power supply |

## Hardware Flow

The overall hardware interaction can be represented as:

```text
Mobile Application
        |
    Bluetooth
        |
        v
      ESP32
   /    |     \
  /     |      \
 v      v       v
L298N  MPU6050  Relay Modules
 |                |
 v                +--> Water Pump
Motors            +--> Air Blower
                  +--> Rotating Brushes
