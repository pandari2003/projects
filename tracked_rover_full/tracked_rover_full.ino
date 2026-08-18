/*
DRV8833	Arduino UNO
AIN1	D10
AIN2	D11
BIN1	D12
BIN2	D13
AOUT1	Motor 1
AOUT2	Motor 1
BOUT1	Motor 2
BOUT2	Motor 2
GND	Arduino GND
VM	Motor battery +

Servo	Signal	Power
AY11	D3	External 5–6 V
AY1	D4	External 5–6 V
BX1	D5	External 5–6 V
P1	D6	External 5–6 V
CY2	D8	External 5–6 V
DX2	D9	External 5–6 V
P2	D7	External 5–6 V

button
Button one side → D2
Button other side → GND

Joysticks1
Joystick	Arduino UNO
AB VRx	A0
AB VRy	A1
AB VCC	5V
AB GND	GND

Joysticks 2
CD VRx	A3
CD VRy	A4
CD VCC	5V
CD GND	GND

Potentiometers

Potentiometer	Arduino UNO

Pot 1 center pin	A2
Pot 1 outer pin	5V
Pot 1 other outer pin	GND

Pot 2 center pin	A5
Pot 2 outer pin	5V
Pot 2 other outer pin	GND
*/
#include <Servo.h>

Servo servoAY1;
Servo servoAY11;
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
const int servoAY11Pin = 3;
const int servoAY1Pin = 4;
const int servoBX1Pin = 5;
const int servoP1Pin = 6;

const int servoCY2Pin = 8;
const int servoDX2Pin = 9;
const int servoP2Pin = 7;

// Control button
const int controlButton = 2;

// indicator
//const int indicator = 3;

// --- DC MOTOR PINS ---
// Motor 1 (Controlled by Joystick CD XY-axis)
const int motor1_IN1 = 10;
const int motor1_IN2 = 11;
const int motor2_IN3 = 12;
const int motor2_IN4 = 13;

// Current servo angles
int ABxAngle = 90;
int AByAngle = 90;

int CDxAngle = 90;
int CDyAngle = 90;

// initial speed dribving mode
int speedValue = 50;

// Joystick settings
const int center = 512;
const int deadZone = 50;

// -------- JOYSTICK FUNCTION --------
void controlJoystick(int pin, int &angle) {

  int value = analogRead(pin);

  if (value > center + deadZone) {
    angle++;
  } else if (value < center - deadZone) {
    angle--;
  }

  angle = constrain(angle, 0, 180);
}

void setup() {

  servoAY1.attach(servoAY1Pin);
  servoAY11.attach(servoAY11Pin);
  servoBX1.attach(servoBX1Pin);
  servoP1.attach(servoP1Pin);

  servoCY2.attach(servoCY2Pin);
  servoDX2.attach(servoDX2Pin);
  servoP2.attach(servoP2Pin);

  // Start servos at 90 degrees
  servoAY1.write(90);
  servoAY11.write(90);
  servoBX1.write(90);
  servoP1.write(90);

  servoCY2.write(90);
  servoDX2.write(90);
  servoP2.write(90);

  // Button
  pinMode(controlButton, INPUT_PULLUP);
  //pinMode(indicator, INPUT_PULLUP);

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
    //digitalWrite(indicator, HIGH);
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
    servoAY11.write(ABxAngle);
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

  // BUTTON OFF = DRIVING MODE

  else {
    //digitalWrite(indicator, LOW);

    Serial.print("DRIVING MODE | ");
    // READ JOYSTICKS
    // AB joystick X = SPEED
    int abXValue = analogRead(joystickABX);

    // CD joystick X = FORWARD / BACKWARD
    int cdXValue = analogRead(joystickCDX);

    // CD joystick Y = LEFT / RIGHT
    int cdYValue = analogRead(joystickCDY);

    // SPEED CONTROL
    // Initial value = 50%
    if (abXValue > center + deadZone) {
      speedValue++;

    } else if (abXValue < center - deadZone) {
      speedValue--;
    }

    // Limit speed from 0 to 100%
    speedValue = constrain(speedValue, 0, 100);

    // Convert percentage to PWM
    int baseSpeed = map(speedValue, 0, 100, 0, 255);

    // CD JOYSTICK X = FORWARD / BACKWARD
    // CD JOYSTICK Y = LEFT / RIGHT

    int leftSpeed = baseSpeed;
    int rightSpeed = baseSpeed;

    // FORWARD
    // CD X+

    if (cdXValue > center + deadZone) {
      // Both motors forward

      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;

      // LEFT MOTOR FORWARD
      analogWrite(motor1_IN1, leftSpeed);
      digitalWrite(motor1_IN2, LOW);

      // RIGHT MOTOR FORWARD
      analogWrite(motor2_IN3, rightSpeed);
      digitalWrite(motor2_IN4, LOW);

      Serial.print("FORWARD ");
    }

    // BACKWARD
    // CD X-

    else if (cdXValue < center - deadZone) {
      // Both motors backward

      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;

      // LEFT MOTOR BACKWARD
      digitalWrite(motor1_IN1, LOW);
      analogWrite(motor1_IN2, leftSpeed);

      // RIGHT MOTOR BACKWARD
      digitalWrite(motor2_IN3, LOW);
      analogWrite(motor2_IN4, rightSpeed);

      Serial.print("BACKWARD ");
    }

    // CD X CENTER
    else {
      // No forward/backward movement
      // RIGHT
      // CD Y+

      if (cdYValue > center + deadZone) {
        // LEFT motor increases
        // RIGHT motor decreases

        leftSpeed = baseSpeed + 50;
        rightSpeed = baseSpeed - 50;

        leftSpeed = constrain(leftSpeed, 0, 255);
        rightSpeed = constrain(rightSpeed, 0, 255);

        // LEFT MOTOR FORWARD
        digitalWrite(motor1_IN1, LOW);
        analogWrite(motor1_IN2, leftSpeed);
        
        // RIGHT MOTOR FORWARD
        analogWrite(motor2_IN3, rightSpeed);
        digitalWrite(motor2_IN4, LOW);

        Serial.print("RIGHT ");
      }

      // LEFT
      // CD Y-

      else if (cdYValue < center - deadZone) {
        // LEFT motor decreases
        // RIGHT motor increases

        leftSpeed = baseSpeed - 50;
        rightSpeed = baseSpeed + 50;

        leftSpeed = constrain(leftSpeed, 0, 255);
        rightSpeed = constrain(rightSpeed, 0, 255);

        // LEFT MOTOR FORWARD
        analogWrite(motor1_IN1, leftSpeed);
        digitalWrite(motor1_IN2, LOW);

        // RIGHT MOTOR FORWARD
        digitalWrite(motor2_IN3, LOW);
        analogWrite(motor2_IN4, rightSpeed);

        Serial.print("LEFT ");
      }

      // CD Y CENTER = STOP
      else {
        digitalWrite(motor1_IN1, LOW);
        digitalWrite(motor1_IN2, LOW);

        digitalWrite(motor2_IN3, LOW);
        digitalWrite(motor2_IN4, LOW);

        Serial.print("STOP ");
      }
    }

    // SERIAL MONITOR

    Serial.print("Speed: ");
    Serial.print(speedValue);
    Serial.print("%");

    Serial.print("  Base PWM: ");
    Serial.print(baseSpeed);

    Serial.print("  Left PWM: ");
    Serial.print(leftSpeed);

    Serial.print("  Right PWM: ");
    Serial.println(rightSpeed);
  }
}