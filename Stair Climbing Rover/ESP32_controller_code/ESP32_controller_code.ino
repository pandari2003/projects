#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// =========================
// JOYSTICK
// =========================
#define JOY_X 34
#define JOY_Y 35

// =========================
// RECEIVER ESP32 MAC
// =========================
uint8_t receiverMAC[] = {
  0xB0, 0xCB, 0xD8, 0x0A, 0x6E, 0xB4
};

// =========================
// DATA SENT TO RECEIVER
// =========================
typedef struct {
  int x;
  int y;
} ControlData;

ControlData data;

void setup() {

  Serial.begin(115200);
  delay(1000);

  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);

  // Start WiFi in Station mode
  WiFi.mode(WIFI_STA);

delay(500);

esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
  Serial.println();
  Serial.println("================================");
  Serial.println("ROVER CONTROLLER");
  Serial.println("================================");

  // We already know the controller MAC.
  // No need to call WiFi.macAddress().
  Serial.println("Controller MAC: B0:CB:D8:0D:05:FC");

  Serial.println("Receiver MAC: B0:CB:D8:0A:6E:B4");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization FAILED!");
    return;
  }

  // Add receiver
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add receiver!");
    return;
  }

  Serial.println("ESP-NOW initialized");
  Serial.println("ROVER CONTROLLER READY");
}

void loop() {

  data.x = analogRead(JOY_X);
  data.y = analogRead(JOY_Y);

  esp_err_t result = esp_now_send(
    receiverMAC,
    (uint8_t *)&data,
    sizeof(data)
  );

  if (result != ESP_OK) {
    Serial.print("ESP-NOW send error: ");
    Serial.println(result);
  }

  Serial.print("X = ");
  Serial.print(data.x);

  Serial.print("   Y = ");
  Serial.println(data.y);

  delay(30);
}