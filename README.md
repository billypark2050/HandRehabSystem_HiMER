# HandRehabSystem_HiMER
1. Data collection system for "Camera DIC to EMG estimation model design"
https://docs.google.com/document/d/16HLX5dg8j9S5Rpnn8MyPvDjhpgtGL0mv7O0paFpKdXU/edit?usp=sharing
# 🚀 6-Axis Hybrid Motor Controller (Arduino)

![System Overview](https://via.placeholder.com/800x400?text=Insert+System+Overview+Image+Here)

This repository contains the Arduino firmware for a **6-axis hybrid motor control system**, capable of simultaneously driving **2 Stepper Motors (X/Y axes)** and **4 Linear DC Motors (Z1-Z4 axes)**.

It uses a robust **non-blocking asynchronous architecture** with Hardware Timer Interrupts, allowing real-time multi-motor coordination via serial commands (e.g., from a Python script or computer vision tracker).

## ✨ Key Features
* **Asynchronous Serial Communication:** Parses incoming serial commands (non-blocking) to update motor target positions instantly without lagging the control loop.
* **Hardware Timer Interrupt (Timer1):** The 4 Linear DC motors are controlled in the background via a 1ms timer interrupt routine (`dcMotorISR`). This guarantees precise time-based actuation (in milliseconds) entirely independent of the stepper motor loop.
* **Simultaneous Stepper Actuation:** Stepper motors (X, Y) are driven in a unified `while` loop, allowing for smooth, simultaneous diagonal movements.
* **Safety Interlocks:** Utilizes `noInterrupts()` and `interrupts()` during variable updates to prevent race conditions when updating the DC motor target times.

## ⚙️ Hardware Requirements
* **Microcontroller:** Arduino Uno / Nano / Mega (must support `TimerOne` library)
* **Stepper Motor Drivers:** 2x A4988, DRV8825, or similar Step/Dir drivers.
* **DC Motor Drivers:** 2x L298N Dual H-Bridge (to control 4 DC motors).
* **Motors:** 2x NEMA 17 Stepper Motors, 4x Linear DC Motors (Actuators).
* **Power Supply:** Appropriate external power for motors (e.g., 12V), sharing a common ground with the Arduino.

## 🔌 Circuit Diagram & Pinout

![Circuit Wiring Diagram](\images\TwoStepperControlwithA4899.png)


### Stepper Motors (X, Y Axes)
| Motor | Step Pin | Dir Pin | Note |
| :--- | :---: | :---: | :--- |
| **X-Axis** | `3` | `2` | Pan / Horizontal |
| **Y-Axis** | `5` | `4` | Tilt / Vertical |

### Linear DC Motors (Z1, Z2, Z3, Z4 Axes)
Driven via L298N Motor Drivers.
*(Note: In the current code, all 4 DC motors are mapped to pins 8 & 9. Ensure you assign unique pins for independent control in your final hardware setup.)*

| Motor | IN1 (Forward) | IN2 (Backward) | Note |
| :--- | :---: | :---: | :--- |
| **Z1** | `8` | `9` | Actuator 1 |
| **Z2** | `TBD` | `TBD` | Actuator 2 |
| **Z3** | `TBD` | `TBD` | Actuator 3 |
| **Z4** | `TBD` | `TBD` | Actuator 4 |

---

## 🚀 How It Works

### The Serial Protocol
The Arduino listens for comma-separated strings ending with a newline character (`\n`) over the Serial port at a baud rate of **9600**.

**Command Format:**
```text
dx,dy,dz1,dz2,dz3,dz4\n
