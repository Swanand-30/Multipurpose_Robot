# **ESP32 Pin Configuration**

This document describes the GPIO configuration and control commands used in the
implemented firmware of the **ESP32 Multi-Purpose Cleaning Robot**.

The ESP32 acts as the main controller and interfaces with the:

- **L298N Motor Driver**
- **MPU6050 Orientation Sensor**
- **Cleaner Relay**
- **Air Blower Relay**
- **Water Pump Relay**
- **Bluetooth Mobile Application**

---

## **1. L298N Motor Driver Connections**

The robot uses an **L298N Dual H-Bridge Motor Driver** to control the drive motors.

| **Signal** | **ESP32 GPIO** | **Function** |
|---|---:|---|
| IN1 | GPIO 14 | Motor direction control |
| IN2 | GPIO 27 | Motor direction control |
| IN3 | GPIO 26 | Motor direction control |
| IN4 | GPIO 13 | Motor direction control |
| ENA | GPIO 12 | PWM speed control |
| ENB | GPIO 5 | PWM speed control |

The **IN1–IN4** pins determine the direction of motor rotation.

The **ENA** and **ENB** pins are used for PWM-based speed control.

---

## **2. PWM Configuration**

The firmware uses the ESP32 PWM peripheral for motor speed control.

| **Parameter** | **Configured Value** |
| PWM Frequency | 30 kHz |
| PWM Resolution | 8-bit |
| PWM Channel 1 | 0 |
| PWM Channel 2 | 1 |
| PWM Value Range | 0–255 |
| Default Motor Speed | 200 |

The motor speed can also be changed by the user during automatic path operation.

---

## **3. Cleaning Mechanism Relay Connections**

Three GPIO pins are used to control the different cleaning mechanisms.

| **Device** | **ESP32 GPIO** | **Purpose** |
| Cleaner / Rotating Brush | GPIO 25 | Turns cleaner ON/OFF |
| Air Blower | GPIO 33 | Turns blower ON/OFF |
| Water Pump | GPIO 15 | Turns water pump ON/OFF |

The relay modules electrically interface the low-power ESP32 GPIO signals with
the higher-power cleaning devices.

---

## **4. MPU6050 Orientation Sensor**

The robot uses an **MPU6050 IMU** for orientation feedback during automatic
movement.

The sensor is initialized using the ESP32 `Wire` interface.

During startup, the firmware performs:

Wire.begin();
mpu.begin();
mpu.calcGyroOffsets();
