#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LSM9DS1.h>

//––– Pin definitions for TTGO LoRa32 T-Beam v1.0 –––
#define LORA_SS   18
#define LORA_RST  14
#define LORA_DIO0 26
#define BATTERY_PIN 35   // ADC1_7 on ESP32

Adafruit_BMP3XX bmp;      // BMP388
Adafruit_LSM9DS1 lsm;     // 9-DOF IMU

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // I2C
  Wire.begin();

  // BMP388 init
  if (!bmp.begin_I2C()) {
    Serial.println("BMP388 init failed!");
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_X2);

  // LSM9DS1 init
  if (!lsm.begin_I2C()) {
    Serial.println("LSM9DS1 init failed!");
    while (1);
  }
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);

  // LoRa init
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  // max range settings
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  Serial.println("Setup complete");
}

void loop() {
  // —— 1) Read acceleration (mg) ——
  lsm.read();
  int16_t ax = (int16_t)(lsm.accelData.x * 1000);
  int16_t ay = (int16_t)(lsm.accelData.y * 1000);
  int16_t az = (int16_t)(lsm.accelData.z * 1000);

  // —— 2) Read temperature (°C ×100) ——
  float temp_c = bmp.readTemperature();
  int16_t t_i = (int16_t)(temp_c * 100);

  // —— 3) Read battery voltage (mV) ——
  // VBAT → voltage divider → ADC pin (0–4095 counts → 0–3.3V)
  float v = analogRead(BATTERY_PIN) * (3.3 / 4095.0) * 2.0;
  uint16_t batt_mv = (uint16_t)(v * 1000);

  // —— 4) Pack into 10-byte buffer ——
  uint8_t buf[10];
  buf[0] = ax >> 8; buf[1] = ax & 0xFF;
  buf[2] = ay >> 8; buf[3] = ay & 0xFF;
  buf[4] = az >> 8; buf[5] = az & 0xFF;
  buf[6] = t_i >> 8; buf[7] = t_i & 0xFF;
  buf[8] = batt_mv >> 8; buf[9] = batt_mv & 0xFF;

  // —— 5) Transmit over LoRa ——
  LoRa.beginPacket();
    LoRa.write(buf, sizeof(buf));
  LoRa.endPacket();

  // —— 6) Debug print ——
  Serial.print("A(mg): "); Serial.print(ax); Serial.print(',');
  Serial.print(ay); Serial.print(','); Serial.print(az);
  Serial.print("  T(C): "); Serial.print(temp_c, 2);
  Serial.print("  Vbat(mV): "); Serial.println(batt_mv);

  delay(1000);
}