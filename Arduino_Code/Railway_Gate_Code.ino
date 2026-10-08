#include <Servo.h>

#define IR_SENSOR1_PIN 2 // IR Sensor 1
#define IR_SENSOR2_PIN 3 // IR Sensor 2

#define GREEN_PIN 6 // Green Traffic Light LED
#define YELLOW_PIN 7 // Yellow Traffic Light LED
#define RED_PIN 8 // Red Traffic Light LED

#define BUZZER_PIN 11 // Buzzer

#define SERVO_PIN1 9 // Servo 1
#define SERVO_PIN2 10 // Servo 2

Servo servo1;
Servo servo2;

enum State {
  INITIAL,
  TRAIN_APPROACHING,
  GATES_CLOSING,
  GATES_CLOSED,
  GATES_OPENING
};

State currentState = INITIAL;

void moveServosSimultaneously(int start1, int end1, int start2, int end2, int delayTime);

void setup() {
  pinMode(IR_SENSOR1_PIN, INPUT);
  pinMode(IR_SENSOR2_PIN, INPUT);

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  servo1.attach(SERVO_PIN1);
  servo2.attach(SERVO_PIN2);

  // Set initial state
  servo1.write(0); // Gate open
  servo2.write(0); // Gate open

  digitalWrite(GREEN_PIN, HIGH); // Green light on
  digitalWrite(BUZZER_PIN, LOW); // Buzzer off

  Serial.begin(9600);
}

void moveServosSimultaneously(int start1, int end1, int start2, int end2, int delayTime) {
  int steps = max(abs(end1 - start1), abs(end2 - start2));
  for (int i = 0; i <= steps; i++) {
    int pos1 = start1 + (end1 - start1) * i / steps;
    int pos2 = start2 + (end2 - start2) * i / steps;
    servo1.write(pos1);
    servo2.write(pos2);
    delay(delayTime);
  }
}

void loop() {
  bool trainOnSensor1 = digitalRead(IR_SENSOR1_PIN);
  bool trainOnSensor2 = digitalRead(IR_SENSOR2_PIN);

  // Debugging output
  Serial.print("Train on Sensor 1: ");
  Serial.print(trainOnSensor1);
  Serial.print(", Train on Sensor 2: ");
  Serial.println(trainOnSensor2);

  switch (currentState) {
    case INITIAL:
      if (trainOnSensor1 && !trainOnSensor2) {
        currentState = TRAIN_APPROACHING;
      }
      break;

    case TRAIN_APPROACHING:
      digitalWrite(GREEN_PIN, LOW); // Turn off green light

      for (int i = 0; i < 3; i++) {
        digitalWrite(YELLOW_PIN, HIGH); // Blink yellow light
        digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
        delay(500);
        digitalWrite(YELLOW_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
        delay(250);
      }

      currentState = GATES_CLOSING;
      break;

    case GATES_CLOSING:
      moveServosSimultaneously(0, 90, 0, 90, 15); // Close both gates simultaneously

      digitalWrite(RED_PIN, HIGH); // Turn on red light

      currentState = GATES_CLOSED;
      break;

    case GATES_CLOSED:
      // Stay in GATES_CLOSED state until Sensor 2 detects the train
      if (!trainOnSensor1 && trainOnSensor2) {
        currentState = GATES_OPENING;
      }
      break;

    case GATES_OPENING:
      digitalWrite(RED_PIN, LOW); // Turn off red light
      digitalWrite(BUZZER_PIN, LOW); // Buzzer off
      digitalWrite(YELLOW_PIN, HIGH);

      moveServosSimultaneously(90, 0, 90, 0, 15); // Open both gates simultaneously

      digitalWrite(YELLOW_PIN, LOW); // Turn off yellow light
      digitalWrite(GREEN_PIN, HIGH); // Green light on

      currentState = INITIAL;
      break;
  }

  delay(100);
}
