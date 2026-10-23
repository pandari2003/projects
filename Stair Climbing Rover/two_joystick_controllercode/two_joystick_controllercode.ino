
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define J1_X 34
#define J1_Y 35
#define J2_X 32
#define J2_Y 33

const uint8_t receiverMAC[] = {
  0xB0, 0xCB, 0xD8, 0x0A, 0x6E, 0xB4
};

typedef struct {
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2;
} ControlData;

ControlData data;

int centerX1, centerY1, centerX2, centerY2;

int readAverage(int pin) {
  long total = 0;

  for (int i = 0; i < 100; i++) {
    total += analogRead(pin);
    delay(5);
  }

  return total / 100;
}

int16_t normalizeAxis(int raw, int center) {
  int delta = raw - center;

  int span = (delta >= 0)
           ? 4095 - center
           : center;

  if (span < 1) span = 1;

  int value = (long)delta * 1000 / span;

  return constrain(value, -1000, 1000);
}

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);

  pinMode(J1_X, INPUT);
  pinMode(J1_Y, INPUT);
  pinMode(J2_X, INPUT);
  pinMode(J2_Y, INPUT);

  Serial.println("Keep both joysticks centered.");
  Serial.println("Calibrating in 2 seconds...");
  delay(2000);

  centerX1 = readAverage(J1_X);
  centerY1 = readAverage(J1_Y);
  centerX2 = readAverage(J2_X);
  centerY2 = readAverage(J2_Y);

  Serial.println("Joystick calibration complete.");
  Serial.printf("Centers: %d, %d, %d, %d\n",
                centerX1, centerY1,
                centerX2, centerY2);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(200);

  // Both ESP32 boards must use channel 1
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Serial.print("Controller MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    while (true) delay(1000);
  }

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMAC, 6);
  peer.channel = 1;
  peer.encrypt = false;

  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Failed to add receiver");
    while (true) delay(1000);
  }

  Serial.println("DUAL JOYSTICK CONTROLLER READY");
}

void loop() {
  data.x1 = normalizeAxis(analogRead(J1_X), centerX1);
  data.y1 = normalizeAxis(analogRead(J1_Y), centerY1);
  data.x2 = normalizeAxis(analogRead(J2_X), centerX2);
  data.y2 = normalizeAxis(analogRead(J2_Y), centerY2);

  esp_now_send(receiverMAC,
               (uint8_t *)&data,
               sizeof(data));

  Serial.printf("J1 X:%d Y:%d | J2 X:%d Y:%d\n",
                data.x1, data.y1,
                data.x2, data.y2);

  delay(30);
}
