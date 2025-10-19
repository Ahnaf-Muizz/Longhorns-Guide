/*
  Robot Car Code with State Machine:
  1. SEEKS an object and stops.
  2. WAITS for an IR "move" command.
  3. Switches to OBSTACLE AVOIDANCE mode.
  4. After avoiding, returns to SEEKING mode.
*/

// --- Include necessary libraries ---
#include <Servo.h>
#include <IRremote.hpp> 

// Servo Motor for Ultrasonic Sensor
Servo myservo;

// Ultrasonic Sensor Pins
int Echo_Pin = A0;  // ECHO pin to A0
int Trig_Pin = A1;  // TRIG pin to A1

// IR Receiver Pin
#define IR_RECEIVER_PIN 12

// L298N Motor Driver Pins
#define Lpwm_pin 5    // ENA: Left motor speed control (PWM)
#define Rpwm_pin 6    // ENB: Right motor speed control (PWM)
int pinLB = 2;      // IN1: Left motor backward
int pinLF = 4;      // IN2: Left motor forward
int pinRB = 7;      // IN3: Right motor backward
int pinRF = 8;      // IN4: Right motor forward

// --- IR Remote Definitions ---
#define IR_STOP_COMMAND 0x45
#define IR_MOVE_COMMAND 0x40

// --- Constants ---
#define SEEK_DISTANCE 15      // Distance in cm to stop in front of an object
#define AVOIDANCE_DISTANCE 20 // Distance in cm to trigger obstacle avoidance
#define FORWARD_SPEED 100     // Default forward speed (0-255)
#define TURN_SPEED 150        // Default turn speed (0-255)

// === State Machine Definition ===
enum RobotState {
  SEEKING,   // Looking for an object
  LOCKED_ON, // Found an object, waiting for command
  AVOIDING,  // Moving and avoiding obstacles
  MANUAL_STOP // Stopped by IR remote
};

// Set the initial state of the robot
RobotState currentState = SEEKING;

// --- Setup Function ---
void setup() {
  myservo.attach(A2);
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);
  myservo.write(90);
  pinMode(Echo_Pin, INPUT);
  pinMode(Trig_Pin, OUTPUT);
  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT);
  pinMode(Rpwm_pin, OUTPUT);
  Serial.println("Robot Initialized. Starting in SEEKING mode.");
}

// --- Main Loop ---
void loop() {
  handleIRCommands();

  switch (currentState) {
    case SEEKING:
      stateSeek();
      break;
    case LOCKED_ON:
      // Waiting for command
      break;
    case AVOIDING:
      stateAvoid();
      break;
    case MANUAL_STOP:
      // Waiting for command
      break;
  }
}


// === State Handling Functions ===

void stateSeek() {
  float distance = checkdistance();
  Serial.print("Seeking... Distance: ");
  Serial.println(distance);

  if (distance < SEEK_DISTANCE && distance > 0) {
    stopp();
    currentState = LOCKED_ON;
    Serial.println("Object found! State -> LOCKED_ON. Waiting for command.");
  } else {
    go_forward(FORWARD_SPEED);
  }
}

void stateAvoid() {
  float Front_Distance = checkdistance();
  Serial.print("Avoiding... Distance: ");
  Serial.println(Front_Distance);

  if (Front_Distance < AVOIDANCE_DISTANCE && Front_Distance > 0) {
    stopp();
    delay(500);

    myservo.write(180); // Look left
    delay(500);
    float Left_Distance = checkdistance();
    delay(100);

    myservo.write(0); // Look right
    delay(500);
    float Right_Distance = checkdistance();
    delay(100);

    myservo.write(90); // Return to center
    delay(200);

    if (Left_Distance > Right_Distance) {
      rotate_left(TURN_SPEED);
      delay(400);
    } else {
      rotate_right(TURN_SPEED);
      delay(400);
    }
  } else {
    // ******************************************************
    // *** THE ONLY CHANGE IS HERE ***
    // If the path is clear, obstacle is avoided. Return to SEEKING mode.
    Serial.println("Path clear! State -> SEEKING.");
    currentState = SEEKING;
    // ******************************************************
  }
}

// --- Helper Functions ---

void handleIRCommands() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.command == IR_STOP_COMMAND) {
      currentState = MANUAL_STOP;
      stopp();
      Serial.println("IR command received! State -> MANUAL_STOP.");
    }
    else if (IrReceiver.decodedIRData.command == IR_MOVE_COMMAND) {
      if (currentState == LOCKED_ON || currentState == MANUAL_STOP) {
        currentState = AVOIDING;
        Serial.println("IR command received! State -> AVOIDING.");
      }
    }
    delay(100);
    IrReceiver.resume();
  }
}

float checkdistance() {
  digitalWrite(Trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_Pin, LOW);
  float distance = pulseIn(Echo_Pin, HIGH) / 58.00;
  delay(10);
  return distance;
}

// --- Motor Control Functions (Unchanged) ---

void go_forward(unsigned char speed_val) {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
  analogWrite(Lpwm_pin, speed_val);
  analogWrite(Rpwm_pin, speed_val);
}

void go_backward(unsigned char speed_val) {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, speed_val);
  analogWrite(Rpwm_pin, speed_val);
}

void rotate_left(unsigned char speed_val) {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, speed_val);
  analogWrite(Rpwm_pin, speed_val);
}

void rotate_right(unsigned char speed_val) {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
  analogWrite(Lpwm_pin, speed_val);
  analogWrite(Rpwm_pin, speed_val);
}

void stopp() {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, LOW);
}