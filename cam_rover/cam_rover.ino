/*
Connections used

Servos

Servo 1 → D22
Servo 2 → D23
Servo 3 → D24
Servo 4 → D25
Servo 5 → D26
Servo 6 → D27

Joystick

X → A0
Y → A1

DRV8833 #1

Motor 1: AIN1 → D2, AIN2 → D30
Motor 2: BIN1 → D3, BIN2 → D31

DRV8833 #2

Motor 3: AIN1 → D4, AIN2 → D32
Motor 4: BIN1 → D5, BIN2 → D33

DRV8833 #3

Motor 5: AIN1 → D6, AIN2 → D34
Motor 6: BIN1 → D7, BIN2 → D35
*/
#include <Servo.h>
 
//                    SERVO OBJECTS
 
Servo servo1;   // FRONT LEFT
Servo servo2;   // FRONT RIGHT

Servo servo3;   // CENTER LEFT
Servo servo4;   // CENTER RIGHT

Servo servo5;   // REAR LEFT
Servo servo6;   // REAR RIGHT
 
//                    SERVO PINS
 
#define SERVO1_PIN 22
#define SERVO2_PIN 23

#define SERVO3_PIN 24
#define SERVO4_PIN 25

#define SERVO5_PIN 26
#define SERVO6_PIN 27
 
//                    JOYSTICK
 
#define JOY_X A0
#define JOY_Y A1

// Normal joystick center
#define JOYSTICK_CENTER 512

// Joystick dead zone
#define DEAD_ZONE 100
 
//                 SERVO ANGLE SETTINGS
 
#define MIN_ANGLE 0
#define MAX_ANGLE 180

#define START_ANGLE 90

// Current steering angle
int currentAngle = START_ANGLE;
 
//                    DRV8833 #1
 
// MOTOR 1
// AIN1 = PWM
// AIN2 = direction
#define M1_IN1 2
#define M1_IN2 30

// MOTOR 2
// BIN1 = PWM
// BIN2 = direction
#define M2_IN1 3
#define M2_IN2 31
 
//                    DRV8833 #2
 
// MOTOR 3
#define M3_IN1 4
#define M3_IN2 32

// MOTOR 4
#define M4_IN1 5
#define M4_IN2 33
 
//                    DRV8833 #3
 
// MOTOR 5
#define M5_IN1 6
#define M5_IN2 34

// MOTOR 6
#define M6_IN1 7
#define M6_IN2 35
 
//                    MOTOR SPEED
 
int motorSpeed = 0;

//             JOYSTICK NOISE FILTER

int readJoystickAverage(int pin)
{
  long total = 0;

  for (int i = 0; i < 10; i++)
  {
    total += analogRead(pin);
    delayMicroseconds(500);
  }

  return total / 10;
}

//                    MOTOR 1
 
void motor1(int speedValue)
{
  speedValue = constrain(speedValue, -255, 255);
  
  // FORWARD
  
  if (speedValue > 0)
  {
    digitalWrite(M1_IN2, LOW);

    analogWrite(
      M1_IN1,
      speedValue
    );
  }
  
  // REVERSE

  else if (speedValue < 0)
  {
    digitalWrite(M1_IN1, LOW);

    digitalWrite(
      M1_IN2,
      HIGH
    );
  }
  
  // STOP

  else
  {
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
  }
}
 
//                    MOTOR 2
 
void motor2(int speedValue)
{
  speedValue = constrain(speedValue, -255, 255);

  // FORWARD

  if (speedValue > 0)
  {
    digitalWrite(M2_IN2, LOW);

    analogWrite(
      M2_IN1,
      speedValue
    );
  }

  // REVERSE

  else if (speedValue < 0)
  {
    digitalWrite(M2_IN1, LOW);

    digitalWrite(
      M2_IN2,
      HIGH
    );
  }

  // STOP

  else
  {
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, LOW);
  }
}
 
//                    MOTOR 3
 
void motor3(int speedValue)
{
  speedValue = constrain(speedValue, -255, 255);

  // FORWARD

  if (speedValue > 0)
  {
    digitalWrite(M3_IN2, LOW);

    analogWrite(
      M3_IN1,
      speedValue
    );
  }

  // REVERSE

  else if (speedValue < 0)
  {
    digitalWrite(M3_IN1, LOW);

    digitalWrite(
      M3_IN2,
      HIGH
    );
  }

  // STOP

  else
  {
    digitalWrite(M3_IN1, LOW);
    digitalWrite(M3_IN2, LOW);
  }
}
 
//                    MOTOR 4
 
void motor4(int speedValue)
{
  speedValue = constrain(speedValue, -255, 255);

  // FORWARD

  if (speedValue > 0)
  {
    digitalWrite(M4_IN2, LOW);

    analogWrite(
      M4_IN1,
      speedValue
    );
  }

  // REVERSE

  else if (speedValue < 0)
  {
    digitalWrite(M4_IN1, LOW);

    digitalWrite(
      M4_IN2,
      HIGH
    );
  }

  // STOP

  else
  {
    digitalWrite(M4_IN1, LOW);
    digitalWrite(M4_IN2, LOW);
  }
}
 
//                    MOTOR 5
 
void motor5(int speedValue)
{
  speedValue = constrain(speedValue, -255, 255);

  // FORWARD

  if (speedValue > 0)
  {
    digitalWrite(M5_IN2, LOW);

    analogWrite(
      M5_IN1,
      speedValue
    );
  }

  // REVERSE

  else if (speedValue < 0)
  {
    digitalWrite(M5_IN1, LOW);

    digitalWrite(
      M5_IN2,
      HIGH
    );
  }

  // STOP

  else
  {
    digitalWrite(M5_IN1, LOW);
    digitalWrite(M5_IN2, LOW);
  }
}
 
//                    MOTOR 6
 
void motor6(int speedValue)
{
  speedValue = constrain(speedValue, -255, 255);

  // FORWARD

  if (speedValue > 0)
  {
    digitalWrite(M6_IN2, LOW);

    analogWrite(
      M6_IN1,
      speedValue
    );
  }

  // REVERSE

  else if (speedValue < 0)
  {
    digitalWrite(M6_IN1, LOW);

    digitalWrite(
      M6_IN2,
      HIGH
    );
  }

  // STOP

  else
  {
    digitalWrite(M6_IN1, LOW);
    digitalWrite(M6_IN2, LOW);
  }
}
 
//                CONTROL ALL 6 MOTORS
 
void moveAllMotors(int speedValue)
{
  motor1(speedValue);
  motor2(speedValue);

  motor3(speedValue);
  motor4(speedValue);

  motor5(speedValue);
  motor6(speedValue);
}
 
//                    STOP MOTORS
 
void stopMotors()
{
  motorSpeed = 0;

  motor1(0);
  motor2(0);

  motor3(0);
  motor4(0);

  motor5(0);
  motor6(0);
}
 
//                  MOVE ALL SERVOS
 
void moveAllServos(int angle)
{
  angle = constrain(
    angle,
    MIN_ANGLE,
    MAX_ANGLE
  );

  // ===================================================
  // LEFT SIDE
  // ===================================================

  servo1.write(180 - angle);
  servo3.write(180 - angle);
  servo5.write(180 - angle);

 // ===================================================
  // RIGHT SIDE
  // ===================================================

  servo2.write(180 - angle);
  servo4.write(180 - angle);
  servo6.write(180 - angle);
}
 
//                       SETUP
 
void setup()
{
  Serial.begin(9600);

  // ===================================================
  // ATTACH SIX SERVOS
  // ===================================================

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);

  servo5.attach(SERVO5_PIN);
  servo6.attach(SERVO6_PIN);

  // ===================================================
  // INITIAL SERVO POSITION
  // ALL SIX = 90°
  // ===================================================

  servo1.write(90);
  servo2.write(90);

  servo3.write(90);
  servo4.write(90);

  servo5.write(90);
  servo6.write(90);

// ===================================================
  // MOTOR PINS
  // ===================================================

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

  // ===================================================
  // STOP ALL MOTORS
  // ===================================================

  stopMotors();

  delay(1000);

  // ===================================================
  // SERIAL MESSAGE
  // ===================================================

  Serial.println();
  Serial.println("==============================");
  Serial.println("6 SERVO + 6 MOTOR ROVER");
  Serial.println("==============================");

  Serial.println("Arduino Mega 2560");

  Serial.println("Servo start position = 90 degrees");

  Serial.println("X- = steering angle DECREASE");
  Serial.println("X+ = steering angle INCREASE");
  Serial.println("X center = HOLD position");

  Serial.println("Y+ = FORWARD");
  Serial.println("Y- = REVERSE");
  Serial.println("Y center = MOTOR OFF");
}
 
//                        LOOP
 
void loop()
{
  // ===================================================
  // READ JOYSTICK
  // ===================================================

int joystickX = readJoystickAverage(JOY_X);
int joystickY = readJoystickAverage(JOY_Y);

  // ===================================================
  //                    X AXIS
  //                 STEERING CONTROL
  // ===================================================

// X- = DECREASE ANGLE

  if (
    joystickX <
    (JOYSTICK_CENTER - DEAD_ZONE)
  )
  {
    currentAngle--;

    currentAngle = constrain(
      currentAngle,
      MIN_ANGLE,
      MAX_ANGLE
    );

    moveAllServos(currentAngle);

    Serial.print("STEERING LEFT  Angle = ");
    Serial.println(currentAngle);

    delay(30);
  }

  // X+ = INCREASE ANGLE
    

  else if (
    joystickX >
    (JOYSTICK_CENTER + DEAD_ZONE)
  )
  {
    currentAngle++;

    currentAngle = constrain(
      currentAngle,
      MIN_ANGLE,
      MAX_ANGLE
    );

    moveAllServos(currentAngle);

    Serial.print("STEERING RIGHT Angle = ");
    Serial.println(currentAngle);

    delay(30);
  }
   
  // X CENTER
    
  //
  // DO NOTHING.
  //
  // The servos stay at their last position.
  //
  // They DO NOT return to 90°.

  // ===================================================
  //                    Y AXIS
  //                 MOTOR CONTROL
  // ===================================================
 
  // Y+ = FORWARD
  if (
    joystickY >
    (JOYSTICK_CENTER + DEAD_ZONE)
  )
  {
    motorSpeed = map(
      joystickY,
      JOYSTICK_CENTER + DEAD_ZONE,
      1023,
      0,
      255
    );

    motorSpeed = constrain(
      motorSpeed,
      0,
      255
    );

    // All six motors forward
    moveAllMotors(motorSpeed);

    Serial.print("FORWARD  Speed = ");
    Serial.print(motorSpeed);

    Serial.print("  ");

    Serial.print(
      map(
        motorSpeed,
        0,
        255,
        0,
        100
      )
    );

    Serial.println("%");
  }
  // Y- = REVERSE

  else if (
    joystickY <
    (JOYSTICK_CENTER - DEAD_ZONE)
  )
  {
    motorSpeed = map(
      joystickY,
      0,
      JOYSTICK_CENTER - DEAD_ZONE,
      255,
      0
    );

    motorSpeed = constrain(
      motorSpeed,
      0,
      255
    );
    // All six motors reverse
    moveAllMotors(-motorSpeed);
    Serial.print("REVERSE  Speed = ");
    Serial.print(motorSpeed);

    Serial.print("  ");

    Serial.print(
      map(
        motorSpeed,
        0,
        255,
        0,
        100
      )
    );

    Serial.println("%");
  }

  // Y CENTER = STOP

  else
  {
    stopMotors();

    Serial.println("MOTORS OFF");
  }

  // Small loop delay
  delay(10);
}