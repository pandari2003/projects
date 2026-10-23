#include <Servo.h>

//====================
// Servo Objects
//====================
Servo leftLeg;
Servo rightLeg;
Servo leftFoot;
Servo rightFoot;

//====================
// Servo Pins
//====================
const int LEFT_LEG_PIN   = 8;
const int RIGHT_LEG_PIN  = 9;
const int LEFT_FOOT_PIN  = 10;
const int RIGHT_FOOT_PIN = 11;

//====================
// Robot Settings
//====================
const int HOME  = 90;
const int STEP  = 25;
const int TILT  = 20;

// Walking Speed
// Larger value = Slower robot
const int SPEED = 300;

//====================
// Robot State
//====================
enum RobotState
{
  STOPPED,
  FORWARD,
  BACKWARD
};

RobotState robotState = STOPPED;

//====================
// Setup
//====================
void setup()
{
  Serial.begin(9600);

  leftLeg.attach(LEFT_LEG_PIN);
  rightLeg.attach(RIGHT_LEG_PIN);
  leftFoot.attach(LEFT_FOOT_PIN);
  rightFoot.attach(RIGHT_FOOT_PIN);

  home();

  Serial.println("================================");
  Serial.println("       CUBY ROBOT");
  Serial.println("================================");
  Serial.println("F = Forward");
  Serial.println("B = Backward");
  Serial.println("S = Stop");
}

//====================
// Main Loop
//====================
void loop()
{
  if (Serial.available())
  {
    char cmd = toupper(Serial.read());

    switch (cmd)
    {
      case 'F':
        robotState = FORWARD;
        Serial.println("Forward");
        break;

      case 'B':
        robotState = BACKWARD;
        Serial.println("Backward");
        break;

      case 'S':
        robotState = STOPPED;
        home();
        Serial.println("Stopped");
        break;
    }
  }

  if (robotState == FORWARD)
  {
    forwardStep();
  }
  else if (robotState == BACKWARD)
  {
    backwardStep();
  }
}

//====================
// Home Position
//====================
void home()
{
  leftLeg.write(HOME);
  rightLeg.write(HOME);
  leftFoot.write(HOME);
  rightFoot.write(HOME);
  delay(SPEED);
}

//====================
// Forward Step
//====================
void forwardStep()
{
  leftFoot.write(HOME + TILT);
  rightFoot.write(HOME - TILT);
  delay(SPEED);

  leftLeg.write(HOME - STEP);
  rightLeg.write(HOME + STEP);
  delay(SPEED);

  leftFoot.write(HOME - TILT);
  rightFoot.write(HOME + TILT);
  delay(SPEED);

  leftLeg.write(HOME + STEP);
  rightLeg.write(HOME - STEP);
  delay(SPEED);
}

//====================
// Backward Step
//====================
void backwardStep()
{
  leftFoot.write(HOME + TILT);
  rightFoot.write(HOME - TILT);
  delay(SPEED);

  leftLeg.write(HOME + STEP);
  rightLeg.write(HOME - STEP);
  delay(SPEED);

  leftFoot.write(HOME - TILT);
  rightFoot.write(HOME + TILT);
  delay(SPEED);

  leftLeg.write(HOME - STEP);
  rightLeg.write(HOME + STEP);
  delay(SPEED);
}