#include <Servo.h>

Servo myServo;

void setup() {
  Serial.begin(115200);
  myServo.attach(10);
//  myServo.attach(3);
  myServo.write(90); // Start at 90 degrees
  Serial.println("Enter angle (0-180):");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the full string until you press Enter
    String input = Serial.readStringUntil('\n'); 
    
    // Remove any accidental spaces or hidden return characters
    input.trim(); 

    // Only process if the user actually typed a number
    if (input.length() > 0) {
      int angle = input.toInt(); // Convert text to number

      // Only move if it is a valid angle
      if (angle >= 0 && angle <= 180) {
        Serial.print("Holding angle at: ");
        Serial.println(angle);
  //      myServo.write(angle);
      } else {
        Serial.println("Invalid! Enter 0 to 180.");
      }
    }
  }
}
