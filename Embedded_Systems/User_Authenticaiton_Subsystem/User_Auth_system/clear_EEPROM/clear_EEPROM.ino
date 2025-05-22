#include <EEPROM.h>

const int EEPROM_SIZE = 512;  // Max 4096 on ESP32
void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0xFF);
  EEPROM.commit();
  Serial.println("EEPROM cleared.");
}
void loop() {
  // Nothing here
}
