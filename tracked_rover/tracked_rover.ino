#include <ESP32Servo.h>

//---------------- Servo Pins ----------------//
#define SERVO_A_PIN 13
#define SERVO_B_PIN 12
#define SERVO_C_PIN 14
#define SERVO_D_PIN 27

//---------------- Joystick Pins ----------------//
// Joy1 A nd D
#define JOY1_X 34
#define JOY1_Y 35

// Joy2 B and C
#define JOY2_X 32
#define JOY2_Y 33

Servo servoA;
Servo servoB;
Servo servoC;
Servo servoD;

// Current angles
float angleA = 90;
float angleB = 90;
float angleC = 90;
float angleD = 90;

// Smooth angles
float targetA = 90;
float targetB = 90;
float targetC = 90;
float targetD = 90;

const int deadZone = 150;
const float speed = 1.0;      // Angle change per loop
const float smooth = 0.25;    // Smooth factor (0.1~0.3)

void setup()
{
  Serial.begin(115200);

  servoA.attach(SERVO_A_PIN);
  servoB.attach(SERVO_B_PIN);
  servoC.attach(SERVO_C_PIN);
  servoD.attach(SERVO_D_PIN);

  servoA.write(90);
  servoB.write(90);
  servoC.write(90);
  servoD.write(90);
}

void loop()
{
  joystick1Control();
  joystick2Control();

  smoothMoveServos();

  delay(10);
}

//======================================================
// Joystick 1 Function
// Up/Down -> Servo A
// Left/Right -> Servo D
//======================================================
void joystick1Control()
{
  int x = analogRead(JOY1_X);
  int y = analogRead(JOY1_Y);

  int dx = x - 2048;
  int dy = y - 2048;

  // Servo A (Up/Down)
  if (dy > deadZone)
    targetA += speed;
  else if (dy < -deadZone)
    targetA -= speed;

  // Servo D (Right/Left)
  if (dx > deadZone)
    targetD += speed;
  else if (dx < -deadZone)
    targetD -= speed;

  targetA = constrain(targetA, 0, 180);
  targetD = constrain(targetD, 0, 180);
}

//======================================================
// Joystick 2 Function
// Up/Down -> Servo B
// Left/Right -> Servo C
//======================================================
void joystick2Control()
{
  int x = analogRead(JOY2_X);
  int y = analogRead(JOY2_Y);

  int dx = x - 2048;
  int dy = y - 2048;

  // Servo B (Up/Down)
  if (dy > deadZone)
    targetB += speed;
  else if (dy < -deadZone)
    targetB -= speed;

  // Servo C (Right/Left)
  if (dx > deadZone)
    targetC += speed;
  else if (dx < -deadZone)
    targetC -= speed;

  targetB = constrain(targetB, 0, 180);
  targetC = constrain(targetC, 0, 180);
}

//======================================================
// Smooth Servo Movement
//======================================================
void smoothMoveServos()
{
  angleA += (targetA - angleA) * smooth;
  angleB += (targetB - angleB) * smooth;
  angleC += (targetC - angleC) * smooth;
  angleD += (targetD - angleD) * smooth;

  servoA.write((int)angleA);
  servoB.write((int)angleB);
  servoC.write((int)angleC);
  servoD.write((int)angleD);
}