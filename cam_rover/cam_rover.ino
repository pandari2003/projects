#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PCA9685

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 150
#define SERVOMAX 600

// Six servo channels
#define SERVO1_CH 0
#define SERVO2_CH 1
#define SERVO3_CH 2
#define SERVO4_CH 3
#define SERVO5_CH 4
#define SERVO6_CH 5

// JOYSTICKS

// STEERING JOYSTICK
// X axis controls all 6 servos
#define STEERING_X A0

// MOTOR JOYSTICK
// Y axis controls all 6 motors
#define MOTOR_X A2
#define MOTOR_Y A3

// JOYSTICK SETTINGS

#define JOY_CENTER 512
#define DEAD_ZONE 100

// STEERING

int currentAngle = 90;

// Maximum steering movement per loop
#define MAX_STEERING_STEP 5

// DRV8833 MOTOR PINS

// DRV8833 #1

// Motor 1
#define M1_IN1 2
#define M1_IN2 30

// Motor 2
#define M2_IN1 3
#define M2_IN2 31

// DRV8833 #2

// Motor 3
#define M3_IN1 4
#define M3_IN2 32

// Motor 4
#define M4_IN1 5
#define M4_IN2 33

// DRV8833 #3

// Motor 5
#define M5_IN1 6
#define M5_IN2 34

// Motor 6
#define M6_IN1 7
#define M6_IN2 35

// SERVO FUNCTION

void writeServo(uint8_t channel, int angle) {
  angle = constrain(angle, 0, 180);

  int pulse = map(
    angle,
    0,
    180,
    SERVOMIN,
    SERVOMAX);

  pwm.setPWM(channel, 0, pulse);
}

// MOVE ALL 6 SERVOS

void moveAllServos(int angle) {
  angle = constrain(angle, 0, 180);

  writeServo(SERVO1_CH, angle);
  writeServo(SERVO2_CH, 180 - angle);
  writeServo(SERVO3_CH, angle);
  writeServo(SERVO4_CH, 180 - angle);
  writeServo(SERVO5_CH, angle);
  writeServo(SERVO6_CH, 180 - angle);
}

// MOTOR 1

void motor1(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(M1_IN2, LOW);
    analogWrite(M1_IN1, speedValue);
  } else if (speedValue < 0) {
    analogWrite(M1_IN1, 0);
    digitalWrite(M1_IN2, HIGH);
  } else {
    analogWrite(M1_IN1, 0);
    digitalWrite(M1_IN2, LOW);
  }
}

// MOTOR 2

void motor2(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(M2_IN2, LOW);
    analogWrite(M2_IN1, speedValue);
  } else if (speedValue < 0) {
    analogWrite(M2_IN1, 0);
    digitalWrite(M2_IN2, HIGH);
  } else {
    analogWrite(M2_IN1, 0);
    digitalWrite(M2_IN2, LOW);
  }
}

// MOTOR 3

void motor3(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(M3_IN2, LOW);
    analogWrite(M3_IN1, speedValue);
  } else if (speedValue < 0) {
    analogWrite(M3_IN1, 0);
    digitalWrite(M3_IN2, HIGH);
  } else {
    analogWrite(M3_IN1, 0);
    digitalWrite(M3_IN2, LOW);
  }
}

// MOTOR 4

void motor4(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(M4_IN2, LOW);
    analogWrite(M4_IN1, speedValue);
  } else if (speedValue < 0) {
    analogWrite(M4_IN1, 0);
    digitalWrite(M4_IN2, HIGH);
  } else {
    analogWrite(M4_IN1, 0);
    digitalWrite(M4_IN2, LOW);
  }
}

// MOTOR 5

void motor5(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(M5_IN2, LOW);
    analogWrite(M5_IN1, speedValue);
  } else if (speedValue < 0) {
    analogWrite(M5_IN1, 0);
    digitalWrite(M5_IN2, HIGH);
  } else {
    analogWrite(M5_IN1, 0);
    digitalWrite(M5_IN2, LOW);
  }
}

// MOTOR 6

void motor6(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(M6_IN2, LOW);
    analogWrite(M6_IN1, speedValue);
  } else if (speedValue < 0) {
    analogWrite(M6_IN1, 0);
    digitalWrite(M6_IN2, HIGH);
  } else {
    analogWrite(M6_IN1, 0);
    digitalWrite(M6_IN2, LOW);
  }
}

// MOVE ALL 6 MOTORS AT SAME SPEED

void moveAllMotors(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  motor1(speedValue);
  motor2(speedValue);
  motor3(speedValue);
  motor4(speedValue);
  motor5(speedValue);
  motor6(speedValue);
}

// STOP ALL MOTORS

void stopMotors() {
  motor1(0);
  motor2(0);
  motor3(0);
  motor4(0);
  motor5(0);
  motor6(0);
}

// SETUP

void setup() {
  Serial.begin(115200);

  // Motor pins

  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);

  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);

  pinMode(M3_IN1, OUTPUT);
  pinMode(M3_IN2, OUTPUT);

  pinMode(M4_IN1, OUTPUT);
  pinMode(M4_IN2, OUTPUT);

  pinMode(M5_IN1, OUTPUT);
  pinMode(M5_IN2, OUTPUT);

  pinMode(M6_IN1, OUTPUT);
  pinMode(M6_IN2, OUTPUT);

  // Stop motors at startup
  stopMotors();

  // PCA9685 setup

  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  delay(100);

  // Start all 6 servos at 90 degrees

  currentAngle = 90;

  moveAllServos(currentAngle);

  delay(500);

  Serial.println("6-Wheel Rover Ready");
}

// LOOP

void loop() {
  // READ TWO JOYSTICKS

  // Steering joystick
  int steeringX = analogRead(STEERING_X);

  // Motor joystick
  int motorY = analogRead(MOTOR_Y);

  // STEERING JOYSTICK

  int steeringDifference = steeringX - JOY_CENTER;

  // STEERING LEFT

  if (steeringDifference < -DEAD_ZONE) {
    int movement = abs(steeringDifference);

    int steeringStep = map(
      movement,
      DEAD_ZONE,
      512,
      1,
      MAX_STEERING_STEP);

    steeringStep = constrain(
      steeringStep,
      1,
      MAX_STEERING_STEP);

    currentAngle -= steeringStep;

    currentAngle = constrain(
      currentAngle,
      0,
      180);

    // All 6 servos move together
    moveAllServos(currentAngle);
  }

  // STEERING RIGHT

  else if (steeringDifference > DEAD_ZONE) {
    int movement = abs(steeringDifference);

    int steeringStep = map(
      movement,
      DEAD_ZONE,
      512,
      1,
      MAX_STEERING_STEP);

    steeringStep = constrain(
      steeringStep,
      1,
      MAX_STEERING_STEP);

    currentAngle += steeringStep;

    currentAngle = constrain(
      currentAngle,
      0,
      180);

    // All 6 servos move together
    moveAllServos(currentAngle);
  }

  // If steering joystick is centered:
  // servos hold their current angle.

  // MOTOR JOYSTICK

  int motorDifference = motorY - JOY_CENTER;

  // FORWARD
  if (motorDifference > DEAD_ZONE) {
    int movement = abs(motorDifference);

    int motorSpeed = map(
      movement,
      DEAD_ZONE,
      512,
      0,
      255);

    motorSpeed = constrain(
      motorSpeed,
      0,
      255);

    // ALL 6 MOTORS SAME SPEED
    moveAllMotors(motorSpeed);
  }

  // REVERSE

  else if (motorDifference < -DEAD_ZONE) {
    int movement = abs(motorDifference);

    int motorSpeed = map(
      movement,
      DEAD_ZONE,
      512,
      0,
      255);

    motorSpeed = constrain(
      motorSpeed,
      0,
      255);

    // ALL 6 MOTORS SAME SPEED
    moveAllMotors(-motorSpeed);
  }

  // MOTOR JOYSTICK CENTER
  else {
    // Stop all 6 motors
    stopMotors();
  }
  // LOOP DELAY

  delay(10);
}