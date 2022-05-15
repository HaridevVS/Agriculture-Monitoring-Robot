/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-dc-motor
 */

#define PIN_IN1  8 // ESP32 pin GIOP27 connected to the IN1 pin L298N
#define PIN_IN2  7 // ESP32 pin GIOP26 connected to the IN2 pin L298N
#define PIN_ENA  6 // ESP32 pin GIOP14 connected to the EN1 pin L298N

// the code in setup function runs only one time when ESP32 starts
void setup() {
  // initialize digital pins as outputs.
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
}

// the code in loop function is executed repeatedly infinitely
void loop() {
  digitalWrite(PIN_IN1, HIGH); // control the motor's direction in clockwise
  digitalWrite(PIN_IN2, LOW);  // control the motor's direction in clockwise

  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(PIN_ENA, speed); // speed up
    delay(10);
  }

  delay(2000); // rotate at maximum speed 2 seconds in clockwise direction

  // change direction
  digitalWrite(PIN_IN1, LOW);   // control the motor's direction in anti-clockwise
  digitalWrite(PIN_IN2, HIGH);  // control the motor's direction in anti-clockwise

  delay(2000); // rotate at maximum speed for 2 seconds in anti-clockwise direction

  for (int speed = 255; speed >= 0; speed--) {
    analogWrite(PIN_ENA, speed); // speed down
    delay(10);
  }

  delay(2000); // stop motor 2 second
}
