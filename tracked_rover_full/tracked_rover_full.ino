#include <Servo.h>

Servo servoAY1;
Servo servoBX1;
Servo servoP1;

Servo servoCY2;
Servo servoDX2;
Servo servoP2;

// Pins
const int joystickABX = A0;
const int joystickABY = A1;
const int potPin1 = A2;

const int joystickCDX = A3;
const int joystickCDY = A4;
const int potPin2 = A5;

// Servo pins
const int servoAY1Pin = 4;
const int servoBX1Pin = 5;
const int servoP1Pin = 6;

const int servoCY2Pin = 8;
const int servoDX2Pin = 9;
const int servoP2Pin = 7;

// Control button
const int controlButton = 2;

// indicator
const int indicator =3;

// --- DC MOTOR PINS ---
// Motor 1 (Controlled by Joystick AB X-axis)
const int motor1_IN1 = 10;
const int motor1_IN2 = 11;

// Motor 2 (Controlled by Joystick CD X-axis)
const int motor2_IN3 = 12;
const int motor2_IN4 = 13;

// Current servo angles
int ABxAngle = 90;
int AByAngle = 90;

int CDxAngle = 90;
int CDyAngle = 90;

// Joystick settings
const int center = 512;
const int deadZone = 50;


// -------- JOYSTICK FUNCTION --------
void controlJoystick(int pin, int &angle) {

  int value = analogRead(pin);

  if (value > center + deadZone) {
    angle++;
  }
  else if (value < center - deadZone) {
    angle--;
  }

  angle = constrain(angle, 0, 180);
}


void setup() {

  servoAY1.attach(servoAY1Pin);
  servoBX1.attach(servoBX1Pin);
  servoP1.attach(servoP1Pin);

  servoCY2.attach(servoCY2Pin);
  servoDX2.attach(servoDX2Pin);
  servoP2.attach(servoP2Pin);


  // Start servos at 90 degrees
  servoAY1.write(90);
  servoBX1.write(90);
  servoP1.write(90);

  servoCY2.write(90);
  servoDX2.write(90);
  servoP2.write(90);

  // Button
  pinMode(controlButton, INPUT_PULLUP);
  pinMode(indicator, INPUT_PULLUP);

  // --- DC MOTOR PIN SETUP ---
  pinMode(motor1_IN1, OUTPUT);
  pinMode(motor1_IN2, OUTPUT);
  pinMode(motor2_IN3, OUTPUT);
  pinMode(motor2_IN4, OUTPUT);

  // Ensure motors start turned off
  digitalWrite(motor1_IN1, LOW);
  digitalWrite(motor1_IN2, LOW);
  digitalWrite(motor2_IN3, LOW);
  digitalWrite(motor2_IN4, LOW);

  // Serial Monitor
  Serial.begin(115200);
}

void loop() {

  // Read button
  int buttonState = digitalRead(controlButton);

  // BUTTON ON

  if (buttonState == LOW) {
    digitalWrite(indicator,HIGH);
    // Stop DC motors immediately when switching to Servo Mode
    digitalWrite(motor1_IN1, LOW);
    digitalWrite(motor1_IN2, LOW);
    digitalWrite(motor2_IN3, LOW);
    digitalWrite(motor2_IN4, LOW);

    // Joystick control
    controlJoystick(joystickABX, ABxAngle);
    controlJoystick(joystickABY, AByAngle);

    controlJoystick(joystickCDX, CDxAngle);
    controlJoystick(joystickCDY, CDyAngle);


    // Move joystick servos
    servoAY1.write(ABxAngle);
    servoBX1.write(AByAngle);

    servoCY2.write(CDxAngle);
    servoDX2.write(CDyAngle);


    // Potentiometer 1
    int pot1Value = analogRead(potPin1);
    int pot1Angle = map(pot1Value, 0, 1023, 0, 180);

    servoP1.write(pot1Angle);

/************************************************/
    // Potentiometer 2
    int pot2Value = analogRead(potPin2);
    int pot2Angle = map(pot2Value, 0, 1023, 0, 180);

    servoP2.write(pot2Angle);

/***********************************************/
    // Serial Monitor
    Serial.print("AB X: ");
    Serial.print(ABxAngle);

    Serial.print("  AB Y: ");
    Serial.print(AByAngle);

    Serial.print("  Pot1: ");
    Serial.print(pot1Angle);

    Serial.print("  |  CD X: ");
    Serial.print(CDxAngle);

    Serial.print("  CD Y: ");
    Serial.print(CDyAngle);

    Serial.print("  Pot2: ");
    Serial.println(pot2Angle);

  }

  // BUTTON OFF

  else {
    digitalWrite(indicator,LOW);
    Serial.print("it is in driving mode: ");

    // Read X axis values for both joysticks
    int abXValue = analogRead(joystickABX);
    int cdXValue = analogRead(joystickCDX);

    // ---- MOTOR 1 CONTROL (Joystick AB X-Axis) ----

    if (abXValue > center + deadZone) {        // Pushed Forward (+)
      digitalWrite(motor1_IN1, HIGH);          // Clockwise
      digitalWrite(motor1_IN2, LOW);
    } 
    else if (abXValue < center - deadZone) {   // Pushed Backward (-)
      digitalWrite(motor1_IN1, LOW);
      digitalWrite(motor1_IN2, HIGH);         // Anti-Clockwise
    } 
    else {                                     // Released / Center
      digitalWrite(motor1_IN1, LOW);           // OFF
      digitalWrite(motor1_IN2, LOW);
    }

    // ---- MOTOR 2 CONTROL (Joystick CD X-Axis) ----

    if (cdXValue > center + deadZone) {        // Pushed Forward (+)
      digitalWrite(motor2_IN3, HIGH);          // Clockwise
      digitalWrite(motor2_IN4, LOW);
    } 
    else if (cdXValue < center - deadZone) {   // Pushed Backward (-)
      digitalWrite(motor2_IN3, LOW);
      digitalWrite(motor2_IN4, HIGH);         // Anti-Clockwise
    } 
    else {                                     // Released / Center
      digitalWrite(motor2_IN3, LOW);           // OFF
      digitalWrite(motor2_IN4, LOW);
    }

  }

  delay(30);
}