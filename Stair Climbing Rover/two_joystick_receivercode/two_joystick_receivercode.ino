
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define UART_RX 16
#define UART_TX 17

typedef struct {
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2;
} ControlData;

void onDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *incomingData,
                int len) {
  if (len != sizeof(ControlData)) return;

  ControlData d;
  memcpy(&d, incomingData, sizeof(d));

  // Send CSV data to Arduino Mega
  Serial1.print(d.x1);
  Serial1.print(',');
  Serial1.print(d.y1);
  Serial1.print(',');
  Serial1.print(d.x2);
  Serial1.print(',');
  Serial1.println(d.y2);

  Serial.printf("J1 X:%d Y:%d | J2 X:%d Y:%d\n",
                d.x1, d.y1, d.x2, d.y2);
}

void setup() {
  Serial.begin(115200);

  Serial1.begin(115200, SERIAL_8N1,
                UART_RX, UART_TX);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(200);

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Serial.println("Receiver MAC:");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    while (true) delay(1000);
  }

  esp_now_register_recv_cb(onDataRecv);

  Serial.println("DUAL JOYSTICK RECEIVER READY");
  Serial.println("Wi-Fi channel: 1");
}

void loop() {
  // Data is handled by the receive callback.
}
