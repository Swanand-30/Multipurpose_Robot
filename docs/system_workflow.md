# **System Workflow and Control Flow**

This document explains the complete operating workflow of the
**ESP32 Multi-Purpose Cleaning Robot**.

The system is designed around an **ESP32 microcontroller** that coordinates:

- Bluetooth communication
- DC motor movement
- PWM-based speed control
- MPU6050 orientation sensing
- Cleaner / rotating brush
- Air blower
- Water pump
- Manual operation
- Automatic path execution

---

# **1. System Overview**

The robot supports two operating modes:

### **Manual Mode**
The user directly controls the robot and cleaning mechanisms through Bluetooth.

### **Auto Path Mode**
The robot executes predefined cleaning paths using programmed movement sequences
and MPU6050-based angular feedback.

The overall control architecture is:

```text
                    Bluetooth Mobile App
                            |
                            v
                          ESP32
            +---------------+---------------+
            |               |               |
            v               v               v
         L298N           MPU6050          Relays
      Motor Driver      Orientation       Control
       /       \          Feedback        /  |  \
      v         v                        v   v   v
 Left Motor  Right Motor              Brush Fan Pump

#System Startup Sequence
Power ON
   |
   v
Initialize Serial Communication
   |
   v
Initialize Bluetooth
   |
   v
Configure Motor GPIO
   |
   v
Configure Relay GPIO
   |
   v
Configure PWM
   |
   v
Initialize MPU6050
   |
   v
Calibrate Gyroscope
   |
   v
Stop Motors
   |
   v
Wait for Bluetooth Connection


