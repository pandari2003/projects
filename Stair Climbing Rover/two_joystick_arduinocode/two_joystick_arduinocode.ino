
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca =
    Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 150
#define SERVOMAX 500

// Joystick deadzone: normalized range -1000 to +1000
#define DEADZONE 250

// Six motor control pins
const int motorPins[6][2] = {
  {30, 31},  // M1 Front Left
  {32, 33},  // M2 Front Right
  {34, 35},  // M3 Middle Left
  {36, 37},  // M4 Middle Right
  {38, 39},  // M5 Rear Left
  {40, 41}   // M6 Rear Right
};

typedef struct {
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2;
} ControlData;

ControlData data;

int angles[6] = {90, 90, 90, 90, 90, 90};

char inputBuffer[50];
byte inputIndex = 0;

unsigned long lastPacketTime = 0;
unsigned long lastServoStep = 0;

const unsigned long SERVO_INTERVAL = 40;
const unsigned long FAILSAFE_MS = 500;

void setServo(int channel, int angle) {
  angle = constrain(angle, 30, 150);
  angles[channel] = angle;

  int pulse = map(angle, 0, 180,
                  SERVOMIN, SERVOMAX);

  pca.setPWM(channel, 0, pulse);
}

void movePair(int first, int second, int direction) {
  if (direction == 0) return;

  // One degree per update
  setServo(first, angles[first] + direction);
  setServo(second, angles[second] + direction);
}

void stopMotors() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(motorPins[i][0], LOW);
    digitalWrite(motorPins[i][1], LOW);
  }
}

void driveMotors(int direction) {
  for (int i = 0; i < 6; i++) {
    if (direction > 0) {
      // Forward
      digitalWrite(motorPins[i][0], HIGH);
      digitalWrite(motorPins[i][1], LOW);
    }
    else if (direction < 0) {
      // Backward
      digitalWrite(motorPins[i][0], LOW);
      digitalWrite(motorPins[i][1], HIGH);
    }
    else {
      digitalWrite(motorPins[i][0], LOW);
      digitalWrite(motorPins[i][1], LOW);
    }
  }
}

void processLine(char *line) {
  int values[4];

  char *token = strtok(line, ",");

  for (int i = 0; i < 4; i++) {
    if (token == NULL) return;

    values[i] = atoi(token);
    token = strtok(NULL, ",");
  }

  data.x1 = constrain(values[0], -1000, 1000);
  data.y1 = constrain(values[1], -1000, 1000);
  data.x2 = constrain(values[2], -1000, 1000);
  data.y2 = constrain(values[3], -1000, 1000);

  lastPacketTime = millis();
}

void setup() {
  Serial.begin(115200);

  // UART from ESP32 receiver
  Serial1.begin(115200);

  for (int i = 0; i < 6; i++) {
    pinMode(motorPins[i][0], OUTPUT);
    pinMode(motorPins[i][1], OUTPUT);
  }

  stopMotors();

  Wire.begin();
  pca.begin();
  pca.setPWMFreq(50);
  delay(500);

  for (int i = 0; i < 6; i++) {
    setServo(i, 90);
  }

  Serial.println("DUAL JOYSTICK ROVER READY");
}

void loop() {
  // Read CSV packets from ESP32 receiver
  while (Serial1.available()) {
    char c = Serial1.read();

    if (c == '\n') {
      inputBuffer[inputIndex] = '\0';
      processLine(inputBuffer);
      inputIndex = 0;
    }
    else if (c != '\r') {
      if (inputIndex < sizeof(inputBuffer) - 1) {
        inputBuffer[inputIndex++] = c;
      } else {
        inputIndex = 0;
      }
    }
  }

  // Stop the motors if wireless data is lost
  if (millis() - lastPacketTime > FAILSAFE_MS) {
    stopMotors();
    return;
  }

  // Joystick 1 Y: forward / backward
  if (data.y1 < -DEADZONE) {
    driveMotors(1);
  }
  else if (data.y1 > DEADZONE) {
    driveMotors(-1);
  }
  else {
    stopMotors();
  }

  // Move servo pairs at a controlled rate
  if (millis() - lastServoStep >= SERVO_INTERVAL) {
    lastServoStep = millis();

    // Joystick 1 X: front pair CH0, CH1
    if (data.x1 < -DEADZONE) {
      movePair(0, 1, -1);
    }
    else if (data.x1 > DEADZONE) {
      movePair(0, 1, 1);
    }

    // Joystick 2 X: middle pair CH2, CH3
    if (data.x2 < -DEADZONE) {
      movePair(2, 3, -1);
    }
    else if (data.x2 > DEADZONE) {
      movePair(2, 3, 1);
    }

    // Joystick 2 Y: rear pair CH4, CH5
    if (data.y2 < -DEADZONE) {
      movePair(4, 5, -1);
    }
    else if (data.y2 > DEADZONE) {
      movePair(4, 5, 1);
    }
  }
}
