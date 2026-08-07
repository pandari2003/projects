// Include the ESP32 Servo library to control servo motors
#include <ESP32Servo.h>

//======================================================
// Servo Pin Definitions
//======================================================

// Servo A is connected to GPIO 13
#define SERVO_A_PIN 13

// Servo B is connected to GPIO 12
#define SERVO_B_PIN 12

// Servo C is connected to GPIO 14
#define SERVO_C_PIN 14

// Servo D is connected to GPIO 27
#define SERVO_D_PIN 27


//======================================================
// Joystick Pin Definitions
//======================================================

// Joystick 1 controls Servo A and Servo D

// X-axis output of Joystick 1 connected to GPIO34
#define JOY1_X 34

// Y-axis output of Joystick 1 connected to GPIO35
#define JOY1_Y 35

// Joystick 2 controls Servo B and Servo C

// X-axis output of Joystick 2 connected to GPIO32
#define JOY2_X 32

// Y-axis output of Joystick 2 connected to GPIO33
#define JOY2_Y 33


//======================================================
// Create Servo Objects
//======================================================

// Create servo object for Servo A
Servo servoA;

// Create servo object for Servo B
Servo servoB;

// Create servo object for Servo C
Servo servoC;

// Create servo object for Servo D
Servo servoD;


//======================================================
// Current Servo Angles
//======================================================

// Stores the current angle of Servo A
float angleA = 90;

// Stores the current angle of Servo B
float angleB = 90;

// Stores the current angle of Servo C
float angleC = 90;

// Stores the current angle of Servo D
float angleD = 90;


//======================================================
// Target Servo Angles
//======================================================

// Desired angle for Servo A
float targetA = 90;

// Desired angle for Servo B
float targetB = 90;

// Desired angle for Servo C
float targetC = 90;

// Desired angle for Servo D
float targetD = 90;


//======================================================
// Control Parameters
//======================================================

// Ignore small joystick movements around the center
const int deadZone = 150;

// Amount the target angle changes in each loop
const float speed = 1.0;

// Smoothness factor
// Smaller value = smoother but slower movement
// Larger value = faster response
const float smooth = 0.25;


//======================================================
// Setup Function
// Runs once after ESP32 powers on
//======================================================
void setup()
{
  // Start Serial Monitor at 115200 baud
  Serial.begin(115200);

  // Attach Servo A to GPIO13
  servoA.attach(SERVO_A_PIN);

  // Attach Servo B to GPIO12
  servoB.attach(SERVO_B_PIN);

  // Attach Servo C to GPIO14
  servoC.attach(SERVO_C_PIN);

  // Attach Servo D to GPIO27
  servoD.attach(SERVO_D_PIN);

  // Move Servo A to center position (90°)
  servoA.write(90);

  // Move Servo B to center position
  servoB.write(90);

  // Move Servo C to center position
  servoC.write(90);

  // Move Servo D to center position
  servoD.write(90);
}


//======================================================
// Main Loop
// Runs continuously
//======================================================
void loop()
{
  // Read Joystick 1 and update Servo A & D targets
  joystick1Control();

  // Read Joystick 2 and update Servo B & C targets
  joystick2Control();

  // Smoothly move all servos toward their targets
  smoothMoveServos();

  // Wait 10 milliseconds
  delay(10);
}


//======================================================
// Joystick 1 Control Function
// Up/Down controls Servo A
// Left/Right controls Servo D
//======================================================
void joystick1Control()
{
  // Read X-axis value (0-4095)
  int x = analogRead(JOY1_X);

  // Read Y-axis value (0-4095)
  int y = analogRead(JOY1_Y);

  // Shift X value so center becomes 0
  int dx = x - 2048;

  // Shift Y value so center becomes 0
  int dy = y - 2048;

  //--------------- Servo A ----------------//

  // If joystick pushed upward beyond dead zone
  if (dy > deadZone)

    // Increase target angle
    targetA += speed;

  // If joystick pushed downward
  else if (dy < -deadZone)

    // Decrease target angle
    targetA -= speed;

  //--------------- Servo D ----------------//

  // If joystick pushed right
  if (dx > deadZone)

    // Increase target angle
    targetD += speed;

  // If joystick pushed left
  else if (dx < -deadZone)

    // Decrease target angle
    targetD -= speed;

  // Limit Servo A angle between 0° and 180°
  targetA = constrain(targetA, 0, 180);

  // Limit Servo D angle between 0° and 180°
  targetD = constrain(targetD, 0, 180);
}


//======================================================
// Joystick 2 Control Function
// Up/Down controls Servo B
// Left/Right controls Servo C
//======================================================
void joystick2Control()
{
  // Read X-axis value
  int x = analogRead(JOY2_X);

  // Read Y-axis value
  int y = analogRead(JOY2_Y);

  // Make joystick center equal to zero
  int dx = x - 2048;

  // Make joystick center equal to zero
  int dy = y - 2048;

  //--------------- Servo B ----------------//

  // Joystick moved upward
  if (dy > deadZone)

    // Increase target angle
    targetB += speed;

  // Joystick moved downward
  else if (dy < -deadZone)

    // Decrease target angle
    targetB -= speed;

  //--------------- Servo C ----------------//

  // Joystick moved right
  if (dx > deadZone)

    // Increase target angle
    targetC += speed;

  // Joystick moved left
  else if (dx < -deadZone)

    // Decrease target angle
    targetC -= speed;

  // Keep Servo B within safe limits
  targetB = constrain(targetB, 0, 180);

  // Keep Servo C within safe limits
  targetC = constrain(targetC, 0, 180);
}


//======================================================
// Smooth Servo Movement Function
// Makes movement gradual instead of jumping
//======================================================
void smoothMoveServos()
{
  // Move Servo A partway toward its target
  angleA += (targetA - angleA) * smooth;

  // Move Servo B partway toward its target
  angleB += (targetB - angleB) * smooth;

  // Move Servo C partway toward its target
  angleC += (targetC - angleC) * smooth;

  // Move Servo D partway toward its target
  angleD += (targetD - angleD) * smooth;

  // Send the calculated angle to Servo A
  servoA.write((int)angleA);

  // Send the calculated angle to Servo B
  servoB.write((int)angleB);

  // Send the calculated angle to Servo C
  servoC.write((int)angleC);

  // Send the calculated angle to Servo D
  servoD.write((int)angleD);
}