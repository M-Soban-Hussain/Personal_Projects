#include <Wire.h>

#define AS5600 0x36
#define RAW_ANGLE 0x0C

float prev = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(AS5600);
  Wire.write(RAW_ANGLE);
  Wire.endTransmission();

  Wire.requestFrom(AS5600, 2);
  if (Wire.available() == 2) {
    int msb = Wire.read();
    int lsb = Wire.read();
    int raw = ((msb << 8) | lsb) & 0x0FFF;

    float angle = raw * 360.0 / 4096.0;

    // smoothing
    angle = angle * 0.7 + prev * 0.3;
    prev = angle;

    Serial.println(angle, 1); // 1 decimal
  }

  delay(250);
}
