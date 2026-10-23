#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// =====================================================          
// PCA9685
// =====================================================

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 150
#define SERVOMAX 500

#define SERVO_MIN_ANGLE 30
#define SERVO_MAX_ANGLE 150

#define SERVO_STEP 2

int servoAngle[6] = {
  90, 90, 90, 90, 90, 90
};

// =====================================================
// JOYSTICK
// =====================================================

#define JOY_X_CENTER 2928
#define JOY_Y_CENTER 2830

#define JOY_DEADZONE 300

// =====================================================
// DRV8833 #1
// M1 = Front Left
// M2 = Front Right
// =====================================================

#define M1_IN1 30   // N1
#define M1_IN2 31   // N2

#define M2_IN1 32   // N3
#define M2_IN2 33   // N4


// =====================================================
// DRV8833 #2
// M3 = Center Left
// M4 = Center Right
// =====================================================

#define M3_IN1 34   // N1
#define M3_IN2 35   // N2

#define M4_IN1 36   // N3
#define M4_IN2 37   // N4


// =====================================================
// DRV8833 #3
// M5 = Rear Left
// M6 = Rear Right
// =====================================================

#define M5_IN1 38   // N1
#define M5_IN2 39   // N2

#define M6_IN1 40   // N3
#define M6_IN2 41   // N4


// =====================================================
// MOTOR FUNCTION
// =====================================================

void motor(int in1, int in2, int direction) {

  if (direction > 0) {

    // FORWARD
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

  }

  else if (direction < 0) {

    // BACKWARD
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

  }

  else {

    // STOP
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}


// =====================================================
// ALL MOTORS FORWARD
// =====================================================

void forward() {

  motor(M1_IN1, M1_IN2, 1);
  motor(M2_IN1, M2_IN2, 1);

  motor(M3_IN1, M3_IN2, 1);
  motor(M4_IN1, M4_IN2, 1);

  motor(M5_IN1, M5_IN2, 1);
  motor(M6_IN1, M6_IN2, 1);

  Serial.println("MOTORS: FORWARD");
}


// =====================================================
// ALL MOTORS BACKWARD
// =====================================================

void backward() {

  motor(M1_IN1, M1_IN2, -1);
  motor(M2_IN1, M2_IN2, -1);

  motor(M3_IN1, M3_IN2, -1);
  motor(M4_IN1, M4_IN2, -1);

  motor(M5_IN1, M5_IN2, -1);
  motor(M6_IN1, M6_IN2, -1);

  Serial.println("MOTORS: BACKWARD");
}


// =====================================================
// STOP ALL MOTORS
// =====================================================

void stopMotors() {

  motor(M1_IN1, M1_IN2, 0);
  motor(M2_IN1, M2_IN2, 0);

  motor(M3_IN1, M3_IN2, 0);
  motor(M4_IN1, M4_IN2, 0);

  motor(M5_IN1, M5_IN2, 0);
  motor(M6_IN1, M6_IN2, 0);

  Serial.println("MOTORS: STOP");
}


// =====================================================
// SERVO ANGLE
// =====================================================

void setServoAngle(uint8_t channel, int angle) {

  angle = constrain(
    angle,
    SERVO_MIN_ANGLE,
    SERVO_MAX_ANGLE
  );

  int pulse = map(
    angle,
    0,
    180,
    SERVOMIN,
    SERVOMAX
  );

  pwm.setPWM(
    channel,
    0,
    pulse
  );
}


// =====================================================
// MOVE ALL SERVOS LEFT
// =====================================================

void moveServosLeft() {

  for (int i = 0; i < 6; i++) {

    servoAngle[i] -= SERVO_STEP;

    servoAngle[i] = constrain(
      servoAngle[i],
      SERVO_MIN_ANGLE,
      SERVO_MAX_ANGLE
    );

    setServoAngle(
      i,
      servoAngle[i]
    );
  }
}


// =====================================================
// MOVE ALL SERVOS RIGHT
// =====================================================

void moveServosRight() {

  for (int i = 0; i < 6; i++) {

    servoAngle[i] += SERVO_STEP;

    servoAngle[i] = constrain(
      servoAngle[i],
      SERVO_MIN_ANGLE,
      SERVO_MAX_ANGLE
    );

    setServoAngle(
      i,
      servoAngle[i]
    );
  }
}


// =====================================================
// CENTER ALL SERVOS
// =====================================================

void centerServos() {

  for (int i = 0; i < 6; i++) {

    servoAngle[i] = 90;

    setServoAngle(
      i,
      servoAngle[i]
    );
  }
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // ESP32 receiver → Mega RX1
  Serial1.begin(115200);


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


  // Start with all motors stopped
  stopMotors();


  // ===================================================
  // PCA9685
  // ===================================================

  pwm.begin();

  pwm.setOscillatorFrequency(27000000);

  pwm.setPWMFreq(50);

  delay(500);


  // Center all servos
  centerServos();


  Serial.println();
  Serial.println("================================");
  Serial.println("6 MOTOR + 6 SERVO ROVER READY");
  Serial.println("================================");
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  if (Serial1.available()) {

    String received = Serial1.readStringUntil('\n');

    received.trim();


    // Find comma between X and Y
    int comma = received.indexOf(',');


    if (comma != -1) {

      int x = received.substring(
        0,
        comma
      ).toInt();

      int y = received.substring(
        comma + 1
      ).toInt();


      Serial.print("X = ");
      Serial.print(x);

      Serial.print("   Y = ");
      Serial.println(y);


      // =============================================
      // MOTOR CONTROL - Y AXIS
      // =============================================

      if (y < JOY_Y_CENTER - JOY_DEADZONE) {

        // UP
        forward();
      }

      else if (y > JOY_Y_CENTER + JOY_DEADZONE) {

        // DOWN
        backward();
      }

      else {

        // CENTER
        stopMotors();
      }


      // =============================================
      // SERVO CONTROL - X AXIS
      // =============================================

      if (x < JOY_X_CENTER - JOY_DEADZONE) {

        // LEFT
        moveServosLeft();
      }

      else if (x > JOY_X_CENTER + JOY_DEADZONE) {

        // RIGHT
        moveServosRight();
      }

      // When X is centered:
      // servos HOLD their current position.
    }
  }
}