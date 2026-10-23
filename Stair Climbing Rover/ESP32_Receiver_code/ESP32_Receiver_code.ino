#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// =====================================================
// UART PINS
// =====================================================

#define RXD2 16
#define TXD2 17

// =====================================================
// DATA STRUCTURE
// =====================================================

typedef struct {
  int x;
  int y;
} ControlData;

ControlData data;

// =====================================================
// ESP-NOW RECEIVE CALLBACK
// =====================================================

void onDataRecv(
  const esp_now_recv_info_t *info,
  const uint8_t *incomingData,
  int len
) {

  if (len == sizeof(ControlData)) {

    memcpy(&data, incomingData, sizeof(data));

    // Send joystick data to Arduino Mega
    Serial1.print(data.x);
    Serial1.print(",");
    Serial1.println(data.y);

    // Display received data
    Serial.print("RECEIVED -> X = ");
    Serial.print(data.x);

    Serial.print("   Y = ");
    Serial.println(data.y);
  }
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  // USB Serial Monitor
  Serial.begin(115200);

  // UART to Arduino Mega
  Serial1.begin(
    115200,
    SERIAL_8N1,
    RXD2,
    TXD2
  );

  delay(1000);

  // ===================================================
  // WIFI
  // ===================================================

  WiFi.mode(WIFI_STA);

  delay(500);

  Serial.println();
  Serial.println("================================");
  Serial.println("ROVER RECEIVER");
  Serial.println("================================");

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  // Set WiFi channel to 1
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Serial.println("WiFi Channel: 1");

  // ===================================================
  // ESP-NOW
  // ===================================================

  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP-NOW initialization FAILED");
    return;
  }

  Serial.println("ESP-NOW initialized");

  // Register receive callback
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("ROVER RECEIVER READY");
  Serial.println("================================");
}

// =====================================================
// LOOP
// =====================================================

void loop() {

}