#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 120
#define SERVOMAX 550
#define SERVO_FREQ 50

#define FL_HIP   0
#define FL_KNEE  1
#define FR_HIP   2
#define FR_KNEE  3
#define BL_HIP   4
#define BL_KNEE  5
#define BR_HIP   6
#define BR_KNEE  7

#define MOVE_DELAY 80

int currentAngle[8] = {
  90, 90, 90, 90,
  90, 90, 90, 90
};

int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void servoWrite(int channel, int angle) {
  pwm.setPWM(channel, 0, angleToPulse(angle));
  currentAngle[channel] = angle;
}

void smoothMove(int channel, int targetAngle) {
  int startAngle = currentAngle[channel];
  if (startAngle < targetAngle) {
    for (int angle = startAngle; angle <= targetAngle; angle++) {
      servoWrite(channel, angle);
      delay(MOVE_DELAY);
    }
  } else {
    for (int angle = startAngle; angle >= targetAngle; angle--) {
      servoWrite(channel, angle);
      delay(MOVE_DELAY);
    }
  }
}

void moveAll(
  int fl_h, int fl_k,
  int fr_h, int fr_k,
  int bl_h, int bl_k,
  int br_h, int br_k
) {
  int targets[8] = {
    fl_h, fl_k,
    fr_h, fr_k,
    bl_h, bl_k,
    br_h, br_k
  };

  bool moving = true;
  while (moving) {
    moving = false;
    for (int i = 0; i < 8; i++) {
      if (currentAngle[i] < targets[i]) {
        currentAngle[i]++;
        servoWrite(i, currentAngle[i]);
        moving = true;
      }
      else if (currentAngle[i] > targets[i]) {
        currentAngle[i]--;
        servoWrite(i, currentAngle[i]);
        moving = true;
      }
    }
    delay(MOVE_DELAY);
  }
}

void Set() {
  moveAll(
    90, 90,
    90, 90,
    90, 90,
    90, 90
  );
}

void Sit() {
  moveAll(
    110, 110,
    60, 60,
    110, 110,
    60, 60
  );
}

void Stand() {
  moveAll(
    80, 80,
    110, 110,
    80, 80,
    110, 110
  );
}
void Walk(){
  moveAll(
    90, 90,
    90, 90,
    90, 90,
    90, 90
  );
}

void setup() {
  Wire.begin();
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  Set();
}

void loop() {
  Set();
  delay(2000);

  Sit();
  delay(2000);

  Stand();
  delay(2000);
}
