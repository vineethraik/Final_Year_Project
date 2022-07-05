#ifdef search_device

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
}

void loop() {
  byte Error, address;
  int DeviceCount;
  Serial.println("Scanning I2C devices...");
  DeviceCount = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    Error = Wire.endTransmission();
    if (Error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      DeviceCount++;
    } else if (Error == 4) {
      Serial.print("Unknown Error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (DeviceCount == 0) {
    Serial.println("No I2C devices found!");
  } else {
    Serial.println("Success!\n");
  }
  delay(5000);
}

#endif