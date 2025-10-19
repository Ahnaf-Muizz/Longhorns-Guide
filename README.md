#Arduino Smart Robot Car with State-Driven Behavior
##Longhorns-Guide
Arduino Smart Robot Car with State-Driven Behavior

This project is an autonomous 2-wheel drive robot car built on the Arduino platform. It uses an ultrasonic sensor and an IR remote to implement a sophisticated, state-driven behavior: it actively seeks an object, stops and waits for a command, navigates around it, and then resumes its seeking behavior.

Features

State Machine Logic: The robot operates using a state machine with four distinct modes: SEEKING, LOCKED_ON, AVOIDING, and MANUAL_STOP.

Autonomous Seeking: The car drives forward on its own, looking for an object to interact with.

Interactive Waiting: Once an object is found within a set distance, the car stops and waits for user input via an IR remote.

Intelligent Obstacle Avoidance: After receiving the 'move' command, the car uses a servo-mounted ultrasonic sensor to scan its surroundings and navigate around the obstacle before resuming its search.

Full IR Remote Control: An IR remote can be used to start the avoidance sequence and to manually stop the car at any time.

Hardware Requirements

To build this project, you will need the following components:

Arduino UNO (or any compatible board)

2WD Robot Car Chassis (with motors and wheels)

L298N Motor Driver Module

HC-SR04 Ultrasonic Distance Sensor

SG90 Micro Servo Motor

A mounting bracket for the servo and ultrasonic sensor

IR Receiver Sensor (like the VS1838B)

A standard IR Remote Control

Power Source (e.g., 4x AA battery pack for motors, 9V battery for Arduino)

Jumper Wires

A computer with the Arduino IDE installed

Software & Libraries

Arduino IDE: Download from the official Arduino website.

Libraries:

<Servo.h>: This library is included with the Arduino IDE by default.

<IRremote.h>: This project requires the IRremote library by shiriff, ArminJo. You can install it from the Arduino IDE's Library Manager (Sketch > Include Library > Manage Libraries...). Search for "IRremote" and install the one by ArminJo.

Wiring and Connections

Connect the components to the Arduino and L298N motor driver according to the pin definitions in the code.

Component

Pin

Connected To

Servo Motor

Signal

Arduino A2

Ultrasonic Sensor

Echo_Pin

Arduino A0



Trig_Pin

Arduino A1

IR Receiver

Signal

Arduino Pin 12

L298N Motor Driver

Lpwm_pin (ENA)

Arduino Pin 5



Rpwm_pin (ENB)

Arduino Pin 6



pinLB (IN1)

Arduino Pin 2



pinLF (IN2)

Arduino Pin 4



pinRB (IN3)

Arduino Pin 7



pinRF (IN4)

Arduino Pin 8



Left Motor

L298N OUT1/OUT2



Right Motor

L298N OUT3/OUT4



Power In

Battery Pack (+)



Ground

Battery Pack (-)

Ensure all components share a common ground (GND).

How It Works: The State Machine

The robot's logic is controlled by a state machine that dictates its behavior at any given moment.

SEEKING (Initial State)

The robot moves forward at FORWARD_SPEED.

It constantly measures the distance in front of it.

If an object is detected within the SEEK_DISTANCE (15 cm), it stops and transitions to the LOCKED_ON state.

LOCKED_ON

The robot remains stationary.

It waits for a command from the IR remote.

If the IR_MOVE_COMMAND is received, it transitions to the AVOIDING state.

AVOIDING

The robot begins its obstacle avoidance maneuver.

It stops, scans left and right using the servo, and measures the distance on both sides.

It turns in the direction with more free space.

Once the path ahead is clear (distance is greater than AVOIDANCE_DISTANCE), it transitions back to the SEEKING state to find a new object.

MANUAL_STOP

This state can be entered at any time by pressing the stop button on the remote (IR_STOP_COMMAND).

The robot will stop all movement and wait until the IR_MOVE_COMMAND is received, at which point it will enter AVOIDING mode.

Configuration & Customization

Before uploading the code, you must configure it for your specific hardware.

1. Finding Your IR Remote Codes

The IR command codes (0x45, 0x40) are unique to my remote. Yours will be different.

How to find your codes: Use the SimpleReceiver or ReceiveDump example sketch that comes with the IRremote library.

Open the Serial Monitor and press the buttons on your remote that you want to use for "move" and "stop".

The monitor will print a Command code (e.g., Cmd: 0x40).

Replace the placeholder values for IR_STOP_COMMAND and IR_MOVE_COMMAND in the main sketch with your codes.

// --- IR Remote Definitions ---
#define IR_STOP_COMMAND 0x45 // <-- REPLACE WITH YOUR CODE
#define IR_MOVE_COMMAND 0x40 // <-- REPLACE WITH YOUR CODE


2. Adjusting Behavior

You can fine-tune the robot's performance by changing these constants at the top of the file:

// --- Constants ---
#define SEEK_DISTANCE 15      // How close to get before stopping (cm)
#define AVOIDANCE_DISTANCE 20 // The minimum clear distance to stop avoiding (cm)
#define FORWARD_SPEED 100     // Speed when seeking (0-255)
#define TURN_SPEED 150        // Speed when turning (0-255)


3. Correcting Motor Drift

If you notice your robot drifting to one side when it should be moving straight, it's because the motors are not perfectly matched. You can correct this by applying a "trim" value. Find the go_forward() function and slightly reduce the speed of the faster motor.

For example, if the car drifts right (left motor is too fast):

// In go_forward()
// Apply a 90% power multiplier to the left motor
analogWrite(Lpwm_pin, speed_val * 0.9); 
analogWrite(Rpwm_pin, speed_val);


How to Use

Assemble the hardware according to the wiring diagram.

Install the Arduino IDE and the required libraries.

Configure the sketch with your unique IR remote command codes.

Upload the code to your Arduino board.

Power on the robot. It will immediately enter SEEKING mode.

Place an object in front of it. It will stop and wait.

Use your remote to start the avoidance maneuver.
