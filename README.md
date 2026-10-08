# Automatic Railway Gate System (ARGS)

A **Semester II Arduino group project** at the University of Information Technology (Group 4, Section B). This tabletop prototype demonstrates a railway crossing that reacts to two infrared sensors and controls two barriers, traffic LEDs, and a buzzer.

## Main components

- Arduino-compatible board and the Arduino Servo library.
- Two IR obstacle sensors representing approach and departure detection.
- Two servo motors operating the model barriers.
- Green, yellow, and red LEDs indicating crossing status.
- Buzzer, suitable power supply, breadboard, resistors, and jumper wiring.

## Wiring used by the sketch

| Component | Arduino pin |
| --- | --- |
| Approach IR sensor | 2 |
| Departure IR sensor | 3 |
| Green LED | 6 |
| Yellow LED | 7 |
| Red LED | 8 |
| Barrier servo 1 | 9 |
| Barrier servo 2 | 10 |
| Buzzer | 11 |

Use suitable current-limiting resistors for LEDs and a common ground. Size the servo power supply for the actual motors rather than assuming the board can power both reliably.

## How it works

The sketch implements five states:

| State | Behavior |
| --- | --- |
| INITIAL | Barriers are open at 0 degrees and the green LED is on. Sensor 1 active with sensor 2 inactive starts the sequence. |
| TRAIN_APPROACHING | Green turns off; the yellow LED and buzzer pulse three times. |
| GATES_CLOSING | Both servos move together from 0 to 90 degrees, then the red LED turns on. |
| GATES_CLOSED | The barriers stay down until sensor 1 is inactive and sensor 2 is active. |
| GATES_OPENING | Red turns off; yellow indicates movement while both servos return to 0 degrees. Green then turns on and the system returns to INITIAL. |

The helper function moves both servos in incremental steps with a 15 ms delay. The main loop prints both sensor values to the serial port and pauses for 100 ms between iterations.

**Sensor polarity matters:** the code treats a HIGH input as detection. Some IR modules use active-LOW outputs; adapt the input interpretation to the module before using the model. The sequence assumes a train travels from sensor 1 toward sensor 2.

## Open and upload

1. Install the Arduino IDE and support for your board.
2. Open Arduino_Code/Railway_Gate_Code.ino. If the IDE asks to place the sketch in a matching folder, accept that organization.
3. Connect the components using the pin table and check servo clearance.
4. Select your board and serial port, then upload the sketch.
5. Open Serial Monitor at **9600 baud** to observe the sensor readings.
6. Trigger sensor 1, then clear sensor 1 and trigger sensor 2 to observe the full sequence.

## Repository structure

Arduino_Code/Railway_Gate_Code.ino contains the complete program: pin definitions, state enumeration, setup, coordinated servo movement, and the main state machine.

## Learning focus and scope

The project demonstrates digital inputs/outputs, servo control, serial debugging, and finite-state control. It uses blocking delays and has no redundant sensing, fault detection, or certified safety controls. It is a classroom model and must not control real railway equipment.

## Credits

Semester II Group 4, Section B project; Aung Myo Pyae is a project contributor. Large archive copies and demonstration videos are omitted from this source repository.
