#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(10);   // Servo signal wire connected to Arduino pin 9

  // Start position
  myServo.write(45);
  delay(500);
}

void loop() {

  // Repeat 5 times
  for (int i = 0; i < 5; i++) {

    // Rotate from 45° to 120°
    for (int angle = 45; angle <= 120; angle++) {
      myServo.write(angle);
      delay(30);
    }

    // Rotate from 120° back to 45°
    for (int angle = 120; angle >= 45; angle--) {
      myServo.write(angle);
      delay(30);
    }
  }

  // Stop at 45° after completing 5 cycles
  myServo.write(45);

  // Prevent loop() from repeating
  while (true) {
    // Do nothing
  }
}