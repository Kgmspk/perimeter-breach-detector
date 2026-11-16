# Perimeter-breach-detector
RESTRICTED ENTRY COUNTDOWN ALARM

An Arduino Uno R3 embedded proximity alert system that monitors a 25 cm restricted zone using an HC-SR04 ultrasonic sensor, triggering a 5-second visual countdown on a Common Anode 7-segment display and an audible buzzer alarm upon intrusion.

HARDWARE COMPONENTS
- Arduino Uno R3
- HC-SR04 Ultrasonic Distance Sensor
- Active Piezo Buzzer
- Common Anode Single-Digit 7-Segment Display (56138Hill)
- 220 Ohm Current-Limiting Resistor(s)
- Breadboard & Jumper Wires

PIN CONNECTIONS

HC-SR04 Ultrasonic Sensor:
- VCC  -> 5V
- Trig -> D9
- Echo -> D10
- GND  -> GND

Buzzer:
- Positive (+) -> D8
- Negative (-) -> GND

7-Segment Display (Common Anode):
- COM (Common Anode) -> 5V (via 220 Ohm resistor)
- a (Top)            -> D2
- b (Top-Right)      -> D3
- c (Bottom-Right)   -> D4
- d (Bottom)         -> D5
- e (Bottom-Left)    -> D6
- g (Middle)         -> D7
- f (Top-Left)       -> D12

HOW IT WORKS
1. Idle Polling: The HC-SR04 sensor polls distance every 50 ms when the zone is clear.
2. Countdown Warning (5 to 1): If an object enters within 25 cm, the display counts down from 5 to 1 with intermittent buzzer beeps while sampling the sensor every 30 ms to verify the target remains present.
3. Auto-Reset on Clearance: If the object leaves the zone for 3 consecutive readings, the buzzer silences immediately and the display clears after a 400 ms hold.
4. Breach Alarm (0): If the target remains through the full countdown, the display latches at 0 and sounds a rapid continuous alarm until 3 consecutive clear readings confirm the area is empty.

HOW TO RUN
1. Connect the components to the Arduino Uno R3 following the pinout above (see Screenshot 2026-11-16 185537.png).
2. Open ar_Monitor--Alarm.ino in the Arduino IDE.
3. Select Arduino Uno and your active COM port, then click Upload.
