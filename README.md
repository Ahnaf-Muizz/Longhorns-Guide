# Arduino Smart Robot Car: Object Follow & Avoidance

This project contains the Arduino code for a 2-wheel drive smart robot car. The robot uses a state machine to intelligently seek out objects, wait for a command, and then navigate around them. It's a great intermediate project for anyone interested in robotics and Arduino programming.



## 🤖 How It Works

The robot operates on a cyclical, state-based logic:

1.  **`SEEKING`**: The robot moves forward, using its ultrasonic sensor to look for an object.
2.  **`LOCKED_ON`**: When an object is detected within a set distance, the robot stops and waits.
3.  **`AVOIDING`**: After receiving a "move" command from an IR remote, the robot switches to obstacle avoidance mode. It scans its surroundings with the servo-mounted sensor and turns toward the clearest path.
4.  **Return to `SEEKING`**: Once the robot has maneuvered and the path ahead is clear, it automatically returns to the `SEEKING` state to find its next target.

An IR remote can also be used to manually stop the robot at any time.

***

## ✨ Features

* **State Machine Logic**: Manages robot behavior cleanly and effectively (`SEEKING`, `LOCKED_ON`, `AVOIDING`, `MANUAL_STOP`).
* **Object Seeking**: Actively moves forward to find and approach an object.
* **Intelligent Avoidance**: Uses a servo to scan left and right to find the best path around an obstacle.
* **IR Remote Control**: Allows for user interaction to start the avoidance sequence and to manually stop the car.
* **Configurable**: Easy-to-change constants for distances and speeds allow for fine-tuning.

***

## 🛠️ Hardware Requirements

* Arduino Uno or any compatible board
* 2WD Robot Car Chassis with DC Motors and Wheels
* L298N Motor Driver Module
* HC-SR04 Ultrasonic Distance Sensor
* SG90 Micro Servo Motor
* IR Receiver Sensor (e.g., VS1838B)
* Standard IR Remote Control
* 9V Battery or other power source for the Arduino and motors
* Jumper Wires

***

## 📚 Libraries

This project requires the following Arduino library:

* **IRremote by shirriff**: For receiving and decoding signals from the IR remote. You can install this through the Arduino IDE's Library Manager.
    * Search for `IRremote` and install the one by Armin Joachimsmeyer / shirriff.

The `Servo.h` library is included with the standard Arduino IDE.

***

## 🔌 Wiring & Pinout

Connect the components to your Arduino according to the pins defined in the code:

| Component             | Pin on Component | Arduino Pin |
| --------------------- | ---------------- | ----------- |
| **Ultrasonic Sensor** | `Trig`           | `A1`        |
|                       | `Echo`           | `A0`        |
| **Servo Motor** | `Signal`         | `A2`        |
| **IR Receiver** | `Signal`         | `12`        |
| **L298N Motor Driver**| `ENA` (Left PWM) | `5`         |
|                       | `ENB` (Right PWM)| `6`         |
|                       | `IN1` (Left Fwd) | `4`         |
|                       | `IN2` (Left Bwd) | `2`         |
|                       | `IN3` (Right Bwd)| `7`         |
|                       | `IN4` (Right Fwd)| `8`         |

*Ensure all components share a common ground (GND) and are powered correctly.*

***

## 🚀 How to Use

### 1. Find Your IR Remote Codes

The most important step is to configure the code for **your specific remote**.

1.  Load an "IR Code Finder" sketch onto your Arduino (many examples are available online with the `IRremote` library).
2.  Point your remote at the IR receiver and press the buttons you want to use for "move" and "stop".
3.  Open the Serial Monitor and note the hexadecimal command codes that are printed.
4.  Update these two lines in the main sketch with your codes:

```cpp
#define IR_STOP_COMMAND 0x45 // REPLACE with your remote's stop command code.
#define IR_MOVE_COMMAND 0x40 // REPLACE with your remote's move command code.
#define SEEK_DISTANCE 15      // How close to get to an object before stopping (cm).
#define AVOIDANCE_DISTANCE 20 // The distance that triggers the avoidance maneuver (cm).
#define FORWARD_SPEED 100     // Speed when seeking (0-255).
#define TURN_SPEED 150        // Speed when turning (0-255).

